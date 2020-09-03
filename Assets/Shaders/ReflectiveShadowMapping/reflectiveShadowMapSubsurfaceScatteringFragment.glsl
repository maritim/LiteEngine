#version 330

layout(location = 0) out vec3 out_color;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;
uniform mat4 modelViewMatrix;
uniform mat4 viewProjectionMatrix;
uniform mat4 modelViewProjectionMatrix;
uniform mat3 normalMatrix;
uniform mat3 normalWorldMatrix;
uniform mat4 inverseViewMatrix;
uniform mat3 inverseNormalWorldMatrix;

uniform vec3 cameraPosition;
uniform vec2 cameraZLimits;

uniform vec2 rsmResolution;
uniform float rsmThickness;
uniform float rsmIndirectRefractiveIntensity;

#include "deferred.glsl"
#include "ReflectiveShadowMapping/reflectiveShadowMapping.glsl"
#include "ScreenSpace/screenSpaceRayTracing.glsl"

vec2 CalcTexCoordRSM ()
{
	return gl_FragCoord.xy / rsmResolution;	
}

vec2 CalcRSMSubsurfaceScattering (vec3 worldPosition, vec3 worldNormal, float in_refractiveIndex)
{
	vec3 viewDirection = normalize (worldPosition - cameraPosition);
	vec3 refractiveDirection = refract (viewDirection, worldNormal, 1.0 / in_refractiveIndex);
	refractiveDirection = normalize (vec3 (lightViewMatrix * vec4 (refractiveDirection, 0.0)));

	vec2 rsmSize = textureSize (rsmPositionMap, 0);

	mat4 pixelProjectionMatrix = mat4 (
		0.5f * rsmSize.x, 0, 0, 0,
		0, 0.5f * rsmSize.y, 0, 0,
		0, 0, 0, 0,
		0.5f * rsmSize.x, 0.5f * rsmSize.y, 0, 1.0f) * lightProjectionMatrix;

	vec2 reflectionPos;
	vec3 reflectionViewPos;

	vec3 rayPosition = vec3 (lightViewMatrix * vec4 (worldPosition, 1.0));
	rayPosition += refractiveDirection * 0.1;

	bool intersect = traceScreenSpaceRay (rayPosition, refractiveDirection, pixelProjectionMatrix,
		rsmPositionMap, rsmSize, rsmThickness, -cameraZLimits.x, 1, 1, 1000,
		1000.0f, reflectionPos, reflectionViewPos);

	if (intersect == false) {
		return vec2 (0.0);
	}

	return reflectionPos / rsmSize;
}

vec3 fresnelSchlick(float cosTheta, vec3 F0)
{
    return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
}

vec3 CalcIndirectRefractiveLight (vec3 in_position, vec3 in_normal, float in_refractiveIndex)
{
	vec3 worldPosition = vec3 (inverseViewMatrix * vec4 (in_position, 1.0));
	vec3 worldNormal = normalMatrix * inverseNormalWorldMatrix * in_normal;

	vec2 reflectionPos = CalcRSMSubsurfaceScattering (worldPosition, worldNormal, in_refractiveIndex);

	vec3 reflectionColor = texture2D (rsmFluxMap, reflectionPos).xyz;

	if (dot (reflectionPos, reflectionPos) == 0) {
		return vec3 (0);
	}

	// vec3 viewDirection = normalize (worldPosition - cameraPosition);
	// vec3 reflectionDirection = normalize (reflect (viewDirection, worldNormal));

	// vec3 fresnel = fresnelSchlick(max(dot(worldNormal, normalize(viewDirection)), 0.0), vec3 (0.0f));

	// vec3 reflectionPosition = texture (rsmPositionMap, reflectionPos).xyz;
	// float d = distance (reflectionPosition, worldPosition);

	return reflectionColor * rsmIndirectRefractiveIntensity;
	// return (reflectionColor * fresnel) / max (d / 3, 1.0f);
}

void main()
{
	vec2 texCoord = CalcTexCoordRSM();
	vec3 in_position = texture2D (gPositionMap, texCoord).xyz;
	vec3 in_normal = texture2D (gNormalMap, texCoord).xyz;
	float in_transparency = texture2D (gDiffuseMap, texCoord).w;
	float in_refractiveIndex = texture2D (gNormalMap, texCoord).w;

	in_normal = normalize(in_normal);

	out_color = vec3 (0.0);

	if (in_transparency > 0.0) {
		out_color = CalcIndirectRefractiveLight (in_position, in_normal, in_refractiveIndex);
	}
} 