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

uniform float rsmThickness;
uniform float rsmIntensity;
uniform float ssrIntensity;
uniform sampler2D ssrPositionMap;

#include "deferred.glsl"
#include "HybridGlobalIllumination/hybridGlobalIllumination.glsl"
#include "ScreenSpace/screenSpaceRayTracing.glsl"

vec2 CalcRSMReflectionPosition (in vec3 worldPosition, in vec3 worldNormal)
{
	vec3 viewDirection = normalize (worldPosition - cameraPosition);

	vec3 reflectionDirection = normalize (reflect (viewDirection, worldNormal));
	reflectionDirection = normalize (vec3 (lightViewMatrix * vec4 (reflectionDirection, 0.0)));

	vec2 rsmSize = textureSize (rsmPositionMap, 0);

	mat4 pixelProjectionMatrix = mat4 (
		0.5f * rsmSize.x, 0, 0, 0,
		0, 0.5f * rsmSize.y, 0, 0,
		0, 0, 0, 0,
		0.5f * rsmSize.x, 0.5f * rsmSize.y, 0, 1.0f) * lightProjectionMatrix;

	vec2 reflectionPos;
	vec3 reflectionViewPos;

	vec3 rayPosition = vec3 (lightViewMatrix * vec4 (worldPosition, 1.0));
	rayPosition += reflectionDirection * 0.1;

	bool intersect = traceScreenSpaceRay (rayPosition, reflectionDirection, pixelProjectionMatrix,
		rsmPositionMap, rsmSize, rsmThickness, -cameraZLimits.x, 1, 0, 1000,
		1000.0f, reflectionPos, reflectionViewPos);

	if (intersect == false) {
		return vec2 (0.0);
	}

	return reflectionPos / rsmSize;
}

vec3 CalcRSMIndirectSpecularLight (vec3 in_position, vec3 in_normal)
{
	vec3 worldPosition = vec3 (inverseViewMatrix * vec4 (in_position, 1.0));
	vec3 worldNormal = normalMatrix * inverseNormalWorldMatrix * in_normal;

	vec2 reflectionPos = CalcRSMReflectionPosition (worldPosition, worldNormal);

	vec3 reflectionColor = texture2D (rsmFluxMap, reflectionPos).xyz;
	vec3 reflectionNormal = normalize (texture2D (rsmNormalMap, reflectionPos).xyz);

	vec3 lightSpaceNormal = normalize (vec3 (lightViewMatrix * vec4 (worldNormal, 0.0)));

	if (dot (lightSpaceNormal, -reflectionNormal) < 0) {
		return vec3 (0);
	}

	return reflectionColor * rsmIntensity;
}

vec3 fresnelSchlick(float cosTheta, vec3 F0)
{
    return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
}

vec3 CalcSSRIndirectSpecularLight (in vec3 in_position, in vec3 in_normal,
	in vec2 reflectionPos, in vec3 reflectionColor)
{
	float screenEdgeFade = 1.0f - clamp (length (vec2 (0.5f) - reflectionPos), 0.0f, 0.5f) * 2.0f;

	vec3 reflectionDirection = normalize (reflect (normalize (in_position), in_normal));

	vec3 reflectionViewPos = textureLod (gPositionMap, reflectionPos, 0).xyz;
	float d = distance (reflectionViewPos, in_position);

	return (reflectionColor * screenEdgeFade *
		clamp (-reflectionDirection.z, 0.0f, 1.0f) * ssrIntensity)
		/ max (d / 3, 1.0f);
}

vec3 CalcIndirectSpecularColor (vec3 in_position, vec3 in_normal, vec2 texCoord)
{
	vec2 reflectionPos = texture (ssrPositionMap, texCoord).xy;

	if (dot (reflectionPos, reflectionPos) <= 0) {
		return CalcRSMIndirectSpecularLight (in_position, in_normal);
	}

	vec3 reflectionColor = texture (directLightMap, reflectionPos).xyz;

	if (dot (reflectionColor, reflectionColor) <= 0) {
		return CalcRSMIndirectSpecularLight (in_position, in_normal);
	}

	return CalcSSRIndirectSpecularLight (in_position, in_normal, reflectionPos, reflectionColor);
}

vec3 CalcIndirectSpecularLight (vec3 in_position, vec3 in_normal, vec2 texCoord)
{
	vec3 reflectionColor = CalcIndirectSpecularColor (in_position, in_normal, texCoord);

	vec3 fresnel = fresnelSchlick(max(dot(in_normal, normalize(-in_position)), 0.0), vec3 (0.0f));

	return reflectionColor * fresnel;
}

void main()
{
	vec2 texCoord = CalcTexCoord();
	vec3 in_position = textureLod (gPositionMap, texCoord, 0).xyz;
	vec3 in_normal = textureLod (gNormalMap, texCoord, 0).xyz;

	in_normal = normalize(in_normal);

	out_color = CalcIndirectSpecularLight(in_position, in_normal, texCoord);
}
