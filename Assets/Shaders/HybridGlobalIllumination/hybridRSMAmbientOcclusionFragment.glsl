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

#include "deferred.glsl"
#include "ReflectiveShadowMapping/reflectiveShadowMapping.glsl"

uniform vec3 lightDirection;

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

float CalcRSMAmbientOcclusion (vec3 in_position, vec3 in_normal, vec2 texCoord, out float dist)
{
	if (in_position == vec3 (0.0)) {
		return 0.0;
	}

	float occlusion = 0.0;

	vec3 lightViewSpacePos = vec3 (lightViewMatrix * inverseViewMatrix * vec4 (in_position, 1.0));
	vec3 lightViewSpaceNormal = vec3 (lightViewMatrix * vec4 (normalMatrix * inverseNormalWorldMatrix * in_normal, 0.0));

	/*
	 * Compute noise scale to map the noise on the whole screen
	*/

	vec2 rsmSize = textureSize (rsmPositionMap, 0);

	vec2 noiseScale = rsmSize / noiseMapSize;

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
		dist += min (ssaoRadius, distance (lightViewSpacePos, sample));

		++ samplesCount;
	}

	samplesCount = max (samplesCount, 1);

	occlusion = 1.0 - (occlusion / samplesCount);

	return occlusion;
}

vec3 CalcAmbientOcclusion (vec3 in_position, vec3 in_normal, vec2 texCoord)
{
	float rsmDist = 0;

	float occlusion = CalcRSMAmbientOcclusion (in_position, in_normal, texCoord, rsmDist);

	float wdRSM = 1.0 - rsmDist / (ssaoSamplesCount * ssaoRadius);
	float wnRSM = max (0, dot (-lightDirection, in_normal));

	float wRSM = hgiAOBlend * wdRSM + (1.0 - hgiAOBlend) * wnRSM;

	occlusion = occlusion * wRSM;

	return vec3 (occlusion, wRSM, 0);
}

void main()
{
	vec2 texCoord = CalcTexCoord();
	vec3 in_position = textureLod (gPositionMap, texCoord, 0).xyz;
	vec3 in_normal = textureLod (gNormalMap, texCoord, 0).xyz;

	in_normal = normalize(in_normal);

	out_color = CalcAmbientOcclusion(in_position, in_normal, texCoord);
}
