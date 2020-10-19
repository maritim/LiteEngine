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

layout(std140) uniform ssaoSamples
{
	int ssaoSamplesCount;
	vec3 ssaoSample[200];
};

uniform sampler2D noiseMap;
uniform ivec2 noiseMapSize;

uniform float ssaoRadius;
uniform float ssaoBias;
uniform float hgiAOBlend;

uniform sampler2D ambientOcclusionMap;

float CalcScreenSpaceAmbientOcclusion (vec3 in_position, vec3 in_normal, vec2 texCoord, out float dist)
{
	float occlusion = 0.0;

	if (in_position == vec3 (0.0)) {
		return 0.0;
	}

	/*
	 * Compute noise scale to map the noise on the whole screen
	*/

	vec2 noiseScale = screenSize / noiseMapSize;

	vec3 randomVec = texture (noiseMap, texCoord * noiseScale).xyz;

	vec3 tangent = normalize (randomVec - in_normal * dot (randomVec, in_normal));
	vec3 bitangent = cross (in_normal, tangent);
	mat3 tangentMatrix = mat3 (tangent, bitangent, in_normal);

	int samplesCount = 0;

	for (int sampleIndex = 0; sampleIndex < ssaoSamplesCount; ++ sampleIndex) {
		vec3 sample = tangentMatrix * ssaoSample [sampleIndex];
		sample = in_position + sample * ssaoRadius;

		vec4 offset = projectionMatrix * vec4 (sample, 1.0);
		offset.xyz /= offset.w;
		offset.xyz = offset.xyz * 0.5 + 0.5;

		if (offset.x < 0 || offset.x > 1 || offset.y < 0 || offset.y > 1) {
			continue;
		}

		vec3 samplePos = textureLod (gPositionMap, offset.xy, 0).xyz;

		float rangeCheck = smoothstep (0.0, 1.0, ssaoRadius / abs (in_position.z - samplePos.z));

		occlusion += (samplePos.z >= sample.z + ssaoBias ? 1.0 : 0.0) * rangeCheck;
		dist += min (ssaoRadius, distance (in_position, sample));

		++ samplesCount;
	}

	samplesCount = max (samplesCount, 1);

	occlusion = 1.0 - (occlusion / samplesCount);

	return occlusion;
}

vec3 CalcSSAO (vec3 in_position, vec3 in_normal, vec2 texCoord)
{
	float ssaoDist = 0;

	float occlusion = CalcScreenSpaceAmbientOcclusion (in_position, in_normal, texCoord, ssaoDist);

	float wdSSAO = 1.0 - ssaoDist / (ssaoSamplesCount * ssaoRadius);
	float wnSSAO = max (0, dot (normalize (- in_position), in_normal));

	float wSSAO = hgiAOBlend * wdSSAO + (1.0 - hgiAOBlend) * wnSSAO;

	occlusion = occlusion * wSSAO;

	return vec3 (occlusion, wSSAO, 0.0);
}

float CalcAmbientOcclusion (vec3 in_position, vec3 in_normal, vec2 texCoord)
{
	vec3 ssao = CalcSSAO (in_position, in_normal, texCoord);
	vec3 rsmAO = texture (ambientOcclusionMap, texCoord).xyz;

	float occlusion = (ssao.x + rsmAO.x) / (ssao.y + rsmAO.y);

	return occlusion;
}

void main()
{
	vec2 texCoord = CalcTexCoord();
	vec3 in_position = textureLod (gPositionMap, texCoord, 0).xyz;
	vec3 in_normal = textureLod (gNormalMap, texCoord, 0).xyz;

	in_normal = normalize(in_normal);

	out_color = CalcAmbientOcclusion(in_position, in_normal, texCoord);
}
