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

uniform float refractiveConeRatio;
uniform float refractiveConeDistance;
uniform float refractiveIndirectIntensity;

#include "deferred.glsl"
#include "VoxelConeTracing/voxelConeTracing.glsl"

vec3 CalcIndirectRefractiveLight (vec3 in_position, vec3 in_normal, float in_refractiveIndex)
{
	// Compute fragment world position and world normal
	vec3 worldPosition = vec3 (inverseViewMatrix * vec4 (in_position, 1.0));
	vec3 worldNormal = normalMatrix * inverseNormalWorldMatrix * in_normal;

	vec3 refractiveLight;

	vec3 eyeToFragment = normalize (worldPosition - cameraPosition);
	vec3 refractiveDir = refract (eyeToFragment, worldNormal, 1.0 / in_refractiveIndex);

	// float refractiveConeRatio = 0.1;

	vec3 refractTraceOrigin = GetPositionInVolume (worldPosition);
	refractiveLight = voxelTraceCone (refractTraceOrigin, refractiveDir, refractiveConeRatio, refractiveConeDistance).xyz;

	return refractiveLight * refractiveIndirectIntensity;
}

void main()
{
	vec2 texCoord = CalcTexCoord();
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