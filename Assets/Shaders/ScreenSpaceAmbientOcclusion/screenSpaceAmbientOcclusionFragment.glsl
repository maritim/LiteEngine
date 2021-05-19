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
uniform vec2 cameraZLimits;

uniform vec2 screenSize;

layout(std140) uniform ssaoSamples
{
	int ssaoSamplesCount;
	vec3 ssaoSample[200];
};

uniform sampler2D noiseMap;
uniform ivec2 noiseMapSize;

uniform vec2 ssaoResolution;
uniform float ssaoRadius;
uniform float ssaoBias;
uniform int ssaoTemporalFilter;

vec2 CalcTexCoord()
{
	return gl_FragCoord.xy / ssaoResolution;
}

float rand(vec2 co){
  return fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453);
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

float CalcScreenSpaceAmbientOcclusion (vec3 in_position, vec3 in_normal, vec2 texCoord)
{
	float occlusion = 0.0;

	/*
	 * Check geometry
	*/

	if (in_position.z <= -cameraZLimits.y) {
		return 0.0;
	}

	/*
	 * Compute noise scale to map the noise on the whole screen
	*/

	vec2 noiseScale = ssaoResolution / noiseMapSize;

	vec3 randomVec = ssaoTemporalFilter <= 0 ?
		texture (noiseMap, texCoord * noiseScale).xyz :
		rand3 (in_position) - rand3 (2 * in_position);

	vec3 tangent = normalize (randomVec - in_normal * dot (randomVec, in_normal));
	vec3 bitangent = cross (in_normal, tangent);
	mat3 tangentMatrix = mat3 (tangent, bitangent, in_normal);

	int samplesCount = 0;

	for (int sampleIndex = 0; sampleIndex < ssaoSamplesCount; ++ sampleIndex) {
		vec3 sample = tangentMatrix * ssaoSample [sampleIndex];
		sample = in_position + sample * ssaoRadius;

		vec4 sampleTexcoord = projectionMatrix * vec4 (sample, 1.0);
		sampleTexcoord.xyz /= sampleTexcoord.w;
		sampleTexcoord.xyz = sampleTexcoord.xyz * 0.5 + 0.5;

		/*
		 * Check sample texcoord limits
		*/

		bvec2 a = greaterThan(sampleTexcoord.xy, vec2(1.0, 1.0));
		bvec2 b = lessThan(sampleTexcoord.xy, vec2(0.0, 0.0));

		if (any(bvec2(any(a), any(b)))) {
			continue;
		}

		vec3 samplePos = textureLod (gPositionMap, sampleTexcoord.xy, 0).xyz;

		float rangeCheck = smoothstep (0.0, 1.0, ssaoRadius / abs (in_position.z - samplePos.z));

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
	vec3 in_position = textureLod (gPositionMap, texCoord, 0).xyz;
	vec3 in_normal = textureLod (gNormalMap, texCoord, 0).xyz;

	in_normal = normalize(in_normal);

	out_color = CalcScreenSpaceAmbientOcclusion (in_position, in_normal, texCoord);
}
