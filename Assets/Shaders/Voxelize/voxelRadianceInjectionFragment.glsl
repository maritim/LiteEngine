#version 420 core

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelViewMatrix;
uniform mat4 projectionMatrix;
uniform mat4 viewProjectionMatrix;
uniform mat4 modelViewProjectionMatrix;
uniform mat3 normalMatrix;
uniform mat3 normalWorldMatrix;
uniform mat4 inverseViewMatrix;
uniform mat3 inverseNormalWorldMatrix;

uniform vec2 rsmResolution;
uniform vec3 lightDirection;

/*
 * Output voxel volume
*/

layout(binding = 0, rgba8) uniform writeonly image3D voxelVolume;

#include "VoxelConeTracing/voxelConeTracing.glsl"
#include "ReflectiveShadowMapping/reflectiveShadowMapping.glsl"

struct RSMSample
{
	vec3 worldSpacePosition;
	vec3 worldSpaceNormal;
	vec3 flux;
	vec3 volumePos;
};

vec2 CalcTexCoord()
{
	return gl_FragCoord.xy / rsmResolution;
}

RSMSample GetRSMSample (vec2 texCoord)
{
	RSMSample rsmSample;

	rsmSample.worldSpacePosition = texture (rsmPositionMap, texCoord).xyz;
	rsmSample.worldSpaceNormal = texture (rsmNormalMap, texCoord).xyz;
	rsmSample.flux = texture (rsmFluxMap, texCoord).xyz;
	rsmSample.volumePos = GetPositionInVolume (rsmSample.worldSpacePosition) * volumeSize;

	return rsmSample;
}

// void CalcBias (inout RSMSample rsmSample)
// {
// 	float volumeCellSize = (maxVertex.x - minVertex.x) / volumeSize.x;

// 	float bias = volumeCellSize * injectionBias;

// 	rsmSample.worldSpacePosition += rsmSample.worldSpaceNormal * bias;
// 	rsmSample.worldSpacePosition += -lightDirection * bias;

// 	rsmSample.volumePos = GetPositionInVolume (rsmSample.worldSpacePosition);
// }

void RadianceInjection (in RSMSample rsmSample)
{
	vec4 voxelColor = texelFetch(voxelTexture, ivec3 (rsmSample.volumePos), 0);

	vec4 fragmentColor = vec4 (rsmSample.flux, voxelColor.a);

	imageStore (voxelVolume, ivec3 (rsmSample.volumePos), fragmentColor);
}

void main ()
{
	vec2 texCoord = CalcTexCoord();

	RSMSample rsmSample = GetRSMSample (texCoord);

	if (dot (rsmSample.volumePos, rsmSample.volumePos) == 0) {
		return;
	}

	RadianceInjection (rsmSample);
}
