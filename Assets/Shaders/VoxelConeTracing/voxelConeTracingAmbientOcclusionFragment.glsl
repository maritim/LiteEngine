#version 330

layout(location = 0) out float out_color;

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

#include "deferred.glsl"
#include "VoxelConeTracing/voxelConeTracing.glsl"

/*
 * Calculate  a vector that is orthogonal to u.
*/

vec3 Orthogonal(vec3 u)
{
	u = normalize(u);

	vec3 v = vec3(0.0, 1.0, 0.0);

	return abs(dot(u, v)) > 0.99 ? cross(u, vec3(0, 0, 1)) : cross(u, v);
}

float CalcOcclusion (vec3 in_position, vec3 in_normal)
{
	// Compute fragment world position and world normal
	vec3 worldPosition = vec3 (inverseViewMatrix * vec4 (in_position, 1.0));
	vec3 worldNormal = normalMatrix * inverseNormalWorldMatrix * in_normal;

	vec3 voxelPos = GetPositionInVolume (worldPosition);

	vec3 tangent = normalize(Orthogonal(worldNormal));
	vec3 bitangent = normalize(cross(worldNormal, tangent));

	float occlusion = 0.0;

	float iblConeRatio = 1;
	float iblMaxDist = .1;

	// this sample gets full weight (dot(normal, normal) == 1)
	occlusion += 1.0 - voxelTraceConeOcclusion(voxelPos, worldNormal, iblConeRatio, iblMaxDist);

	// these samples get partial weight
	occlusion += 1.0 - voxelTraceConeOcclusion(voxelPos, normalize(worldNormal + tangent), iblConeRatio, iblMaxDist);
	occlusion += 1.0 - voxelTraceConeOcclusion(voxelPos, normalize(worldNormal - tangent), iblConeRatio, iblMaxDist);
	occlusion += 1.0 - voxelTraceConeOcclusion(voxelPos, normalize(worldNormal + bitangent), iblConeRatio, iblMaxDist);
	occlusion += 1.0 - voxelTraceConeOcclusion(voxelPos, normalize(worldNormal - bitangent), iblConeRatio, iblMaxDist);

	// Return result.
	return occlusion / 5.0;
}

void main()
{
	vec2 texCoord = CalcTexCoord();
	vec3 in_position = textureLod (gPositionMap, texCoord, 0).xyz;
	vec3 in_normal = textureLod (gNormalMap, texCoord, 0).xyz;

	in_normal = normalize(in_normal);

	out_color = CalcOcclusion (in_position, in_normal);
} 