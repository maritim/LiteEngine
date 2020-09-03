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

uniform sampler3D geometryTexture;

uniform ivec3 volumeSize;
uniform vec3 minVertex;
uniform vec3 maxVertex;

#include "deferred.glsl"

float GetInterpolatedComp (float comp, float minValue, float maxValue)
{
	return ((comp - minValue) / (maxValue - minValue));
}

vec3 GetPositionInVolume (vec3 origin)
{
	vec3 positionInVolume;

	positionInVolume.x = GetInterpolatedComp (origin.x, minVertex.x, maxVertex.x);
	positionInVolume.y = GetInterpolatedComp (origin.y, minVertex.y, maxVertex.y);
	positionInVolume.z = GetInterpolatedComp (origin.z, minVertex.z, maxVertex.z);

	return positionInVolume;
}

/*
 * Calculate  a vector that is orthogonal to u.
*/

vec3 Orthogonal(vec3 u)
{
	u = normalize(u);

	vec3 v = vec3(0.0, 1.0, 0.0);

	return abs(dot(u, v)) > 0.99 ? cross(u, vec3(0, 0, 1)) : cross(u, v);
}

/*Spherical harmonics coefficients - precomputed*/
#define SH_C0 0.282094792f // 1 / 2sqrt(pi)
#define SH_C1 0.488602512f // sqrt(3/pi) / 2

/*Cosine lobe coeff*/
#define SH_cosLobe_C0 0.886226925f // sqrt(pi)/2 
#define SH_cosLobe_C1 1.02332671f // sqrt(pi/3) 

vec4 evalSH_direct( vec3 direction ) {	
	return vec4( SH_C0, -SH_C1 * direction.y, SH_C1 * direction.z, -SH_C1 * direction.x );
}

vec3 volumeCellSize = 1.0f / volumeSize;

float CalcOcclusion (vec3 volumePos, vec3 direction)
{
	vec3 occCoord = (vec3( volumePos ) + 0.5 * direction) * volumeCellSize;

	vec4 occCoefficients = texture (geometryTexture, occCoord);
	float occlusion = 1.0 - clamp (dot (occCoefficients, evalSH_direct( -direction )), 0.0, 1.0);

	return occlusion;
}

float CalcAmbientOcclusion (vec3 in_position, vec3 in_normal)
{
	// Compute fragment world position and world normal
	vec3 worldPosition = vec3 (inverseViewMatrix * vec4 (in_position, 1.0));
	vec3 worldNormal = normalMatrix * inverseNormalWorldMatrix * in_normal;

	vec3 volumePos = GetPositionInVolume (worldPosition);

	vec3 tangent = normalize(Orthogonal(worldNormal));
	vec3 bitangent = normalize(cross(worldNormal, tangent));

	float occlusion = 0.0;

	// this sample gets full weight (dot(normal, normal) == 1)
	occlusion += CalcOcclusion (volumePos, worldNormal);

	// these samples get partial weight
	occlusion += CalcOcclusion (volumePos, normalize(worldNormal + tangent));
	occlusion += CalcOcclusion (volumePos, normalize(worldNormal - tangent));
	occlusion += CalcOcclusion (volumePos, normalize(worldNormal + bitangent));
	occlusion += CalcOcclusion (volumePos, normalize(worldNormal - bitangent));

	// Return result.
	return occlusion / 5.0;
}

void main()
{
	vec2 texCoord = CalcTexCoord();
	vec3 in_position = texture2D (gPositionMap, texCoord).xyz;
	vec3 in_normal = texture2D (gNormalMap, texCoord).xyz;

	in_normal = normalize(in_normal);

	out_color = CalcAmbientOcclusion (in_position, in_normal);
} 