#version 330 core

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

layout(std140) uniform ssaoSamples
{
	int ssaoSamplesCount;
	vec3 ssaoSample[200];
};

uniform sampler2D noiseMap;
uniform vec2 noiseSize;

uniform float ssaoRadius;
uniform float ssaoBias;

#include "deferred.glsl"
#include "ReflectiveShadowMapping/reflectiveShadowMapping.glsl"

float CalcRSMAmbientOcclusion (vec3 in_position, vec3 in_normal, vec2 texCoord)
{
	vec3 lightViewSpacePos = vec3 (lightViewMatrix * inverseViewMatrix * vec4 (in_position, 1.0));
	vec3 lightViewSpaceNormal = vec3 (lightViewMatrix * vec4 (normalMatrix * inverseNormalWorldMatrix * in_normal, 0.0));

	float occlusion = 0.0;

	if (in_position == vec3 (0.0)) {
		return 0.0;
	}

	/*
	 * Compute noise scale to map the noise on the whole screen
	*/

	vec2 rsmSize = textureSize (rsmPositionMap, 0);

	vec2 noiseScale = rsmSize / noiseSize;

	vec3 randomVec = texture (noiseMap, texCoord * noiseScale).xyz;

	vec3 tangent = normalize (randomVec - lightViewSpaceNormal * dot (randomVec, lightViewSpaceNormal));
	vec3 bitangent = cross (lightViewSpaceNormal, tangent);
	mat3 tangentMatrix = mat3 (tangent, bitangent, lightViewSpaceNormal);

	int samplesCount = 0;

	for (int sampleIndex = 0; sampleIndex < ssaoSamplesCount; ++ sampleIndex) {
		vec3 sample = tangentMatrix * ssaoSample [sampleIndex];
		sample = lightViewSpacePos + sample * ssaoRadius;

		vec4 offset = lightProjectionMatrix * vec4 (sample, 1.0);
		offset.xyz /= offset.w;
		offset.xyz = offset.xyz * 0.5 + 0.5;

		if (offset.x < 0 || offset.x > 1 || offset.y < 0 || offset.y > 1) {
			continue;
		}

		vec3 samplePos = texture2D (rsmPositionMap, offset.xy).xyz;

		float rangeCheck = smoothstep (0.0, 1.0, ssaoRadius / abs (lightViewSpacePos.z - samplePos.z));

		occlusion += (samplePos.z >= sample.z + ssaoBias ? 1.0 : 0.0) * rangeCheck;

		++ samplesCount;
	}

	samplesCount = max (samplesCount, 1);

	occlusion = 1.0 - (occlusion / samplesCount);

	return occlusion;
}

void main()
{
	vec2 texCoord = CalcTexCoord();
	vec3 in_position = texture2D (gPositionMap, texCoord).xyz;
	vec3 in_normal = texture2D (gNormalMap, texCoord).xyz;

	in_normal = normalize(in_normal);

	out_color = CalcRSMAmbientOcclusion (in_position, in_normal, texCoord);
}
