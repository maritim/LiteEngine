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

uniform float aoConeRatio;
uniform float aoConeDistance;

uniform int temporalFilterEnabled;

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

vec3 rand3(vec3 p)
{
	vec3 q = vec3(
		dot(p, vec3(127.1, 311.7, 74.7)),
		dot(p, vec3(269.5, 183.3, 246.1)),
		dot(p, vec3(113.5, 271.9, 124.6))
		);

	return fract(sin(q) * 43758.5453123);
}

float CalcOcclusion (vec3 in_position, vec3 in_normal)
{
	// Compute fragment world position and world normal
	vec3 worldPosition = vec3 (inverseViewMatrix * vec4 (in_position, 1.0));
	vec3 worldNormal = normalMatrix * inverseNormalWorldMatrix * in_normal;

	vec3 voxelPos = GetPositionInVolume (worldPosition) + worldNormal * originBias;

	vec3 randomVec = temporalFilterEnabled <= 0 ? vec3 (1, 0, 0) :
		normalize (rand3 (in_position));

	vec3 tangent = normalize (randomVec - worldNormal * dot (randomVec, worldNormal));


	// vec3 tangent = normalize(Orthogonal(worldNormal));
	vec3 bitangent = normalize(cross(worldNormal, tangent));

	float occlusion = 0.0;

	// this sample gets full weight (dot(normal, normal) == 1)
	occlusion += 1.0 - voxelTraceConeOcclusion(voxelPos, worldNormal, aoConeRatio, aoConeDistance);

	// these samples get partial weight
	occlusion += 1.0 - voxelTraceConeOcclusion(voxelPos, normalize(worldNormal + tangent), aoConeRatio, aoConeDistance);
	occlusion += 1.0 - voxelTraceConeOcclusion(voxelPos, normalize(worldNormal - tangent), aoConeRatio, aoConeDistance);
	occlusion += 1.0 - voxelTraceConeOcclusion(voxelPos, normalize(worldNormal + bitangent), aoConeRatio, aoConeDistance);
	occlusion += 1.0 - voxelTraceConeOcclusion(voxelPos, normalize(worldNormal - bitangent), aoConeRatio, aoConeDistance);

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