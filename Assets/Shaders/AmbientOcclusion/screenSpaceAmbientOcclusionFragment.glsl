#version 330 core

layout(location = 0) out float out_color;

uniform sampler2D gPositionMap;
uniform sampler2D gNormalMap;
uniform sampler2D gDiffuseMap;
uniform sampler2D gSpecularMap;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelViewMatrix;
uniform mat4 projectionMatrix;
uniform mat4 modelViewProjectionMatrix;
uniform mat3 normalMatrix;
uniform mat3 normalWorldMatrix;

uniform vec3 cameraPosition;

uniform vec3 lightPosition;
uniform vec3 lightColor;

uniform vec2 screenSize;

const int SSAO_SAMPLES_KERNEL = 64;

uniform int ssaoSamplesCount;
uniform vec3 ssaoSample[SSAO_SAMPLES_KERNEL];

uniform sampler2D ssaoNoiseMap;

uniform vec2 ssaoNoiseSize;

uniform float ssaoRadius;
uniform float ssaoBias;

vec2 CalcTexCoord()
{
	return gl_FragCoord.xy / screenSize;
}

float CalcScreenSpaceAmbientOcclusion (vec3 in_position, vec3 in_normal, vec2 texCoord)
{
	float occlusion = 0.0;

	/*
	 * Compute noise scale to map the noise on the whole screen
	*/

	vec2 noiseScale = screenSize / ssaoNoiseSize;

	vec3 randomVec = texture (ssaoNoiseMap, texCoord * noiseScale).xyz;

	vec3 tangent = normalize (randomVec - in_normal * dot (randomVec, in_normal));
	vec3 bitangent = cross (in_normal, tangent);
	mat3 tangentMatrix = mat3 (tangent, bitangent, in_normal);

	for (int sampleIndex = 0; sampleIndex < ssaoSamplesCount; ++ sampleIndex) {
		vec3 sample = tangentMatrix * ssaoSample [sampleIndex];
		sample = in_position + sample * ssaoRadius;

		vec4 offset = projectionMatrix * vec4 (sample, 1.0);
		offset.xyz /= offset.w;
		offset.xyz = offset.xyz * 0.5 + 0.5;

		vec3 samplePos = texture2D (gPositionMap, offset.xy).xyz;

		float rangeCheck = smoothstep (0.0, 1.0, ssaoRadius / abs (in_position.z - samplePos.z));

		occlusion += (samplePos.z >= sample.z + ssaoBias ? 1.0 : 0.0) * rangeCheck;
	}

	occlusion = 1.0 - (occlusion / ssaoSamplesCount);

	return occlusion;
}

void main()
{
	vec2 texCoord = CalcTexCoord();
	vec3 in_position = texture2D (gPositionMap, texCoord).xyz;
	vec3 in_normal = texture2D (gNormalMap, texCoord).xyz;

	in_normal = normalize(in_normal);

	out_color = CalcScreenSpaceAmbientOcclusion (in_position, in_normal, texCoord);
}
