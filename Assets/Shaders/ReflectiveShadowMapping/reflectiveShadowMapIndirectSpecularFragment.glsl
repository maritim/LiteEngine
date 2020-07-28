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
uniform float rsmIntensity;

#include "deferred.glsl"
#include "ReflectiveShadowMapping/reflectiveShadowMapping.glsl"
#include "ScreenSpace/screenSpaceRayTracing.glsl"

vec2 CalcTexCoordRSM ()
{
	return gl_FragCoord.xy / rsmResolution;	
}

vec2 CalcRSMReflection (vec3 worldPosition, vec3 worldNormal)
{
	vec3 viewDirection = normalize (worldPosition - cameraPosition);

	vec3 reflectionDirection = normalize (reflect (viewDirection, worldNormal));

	vec2 rsmSize = textureSize (rsmPositionMap, 0);

	mat4 pixelProjectionMatrix = mat4 (
		0.5 * rsmSize.x, 0, 0, 0,
		0, 0.5 * rsmSize.y, 0, 0,
		0, 0, 0, 0,
		0.5 * rsmSize.x, 0.5 * rsmSize.y, 0, 1.0f)
		* inverse (mat4 (
			0.5, 0, 0, 0,
			0, 0.5, 0, 0,
			0, 0, 0.5, 0,
			0.5, 0.5, 0.5, 1.0f
		)) * lightSpaceMatrix;

	vec2 reflectionPos;
	vec3 reflectionViewPos;

	vec3 rayPosition = worldPosition + reflectionDirection * 0.1;

	bool intersect = traceScreenSpaceRay (rayPosition, reflectionDirection, pixelProjectionMatrix,
		rsmPositionMap, rsmSize, rsmThickness, -cameraZLimits.x, 1, 0, 1000,
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

vec3 CalcRSMIndirectSpecular (vec3 in_position, vec3 in_normal)
{
	vec3 worldPosition = vec3 (inverseViewMatrix * vec4 (in_position, 1.0));
	vec3 worldNormal = normalMatrix * inverseNormalWorldMatrix * in_normal;

	vec2 reflectionPos = CalcRSMReflection (worldPosition, worldNormal);

	vec3 reflectionColorColor = texture2D (rsmFluxMap, reflectionPos).xyz;
	vec3 reflectionNormal = normalize (texture2D (rsmNormalMap, reflectionPos).xyz);

	if (dot (worldNormal, -reflectionNormal) < 0) {
		return vec3 (0);
	}

	vec3 viewDirection = normalize (worldPosition - cameraPosition);
	vec3 reflectionDirection = normalize (reflect (viewDirection, worldNormal));

	vec3 fresnel = fresnelSchlick(max(dot(worldNormal, normalize(viewDirection)), 0.0), vec3 (0.0f));

	vec3 reflectionPosition = texture (rsmPositionMap, reflectionPos).xyz;
	float d = distance (reflectionPosition, worldPosition);

	return (reflectionColorColor * fresnel) / max (d / 3, 1.0f);
}

void main()
{
	vec2 texCoord = CalcTexCoordRSM();
	vec3 in_position = texture2D (gPositionMap, texCoord).xyz;
	vec3 in_normal = texture2D (gNormalMap, texCoord).xyz;

	in_normal = normalize(in_normal);

	out_color = CalcRSMIndirectSpecular (in_position, in_normal);
} 