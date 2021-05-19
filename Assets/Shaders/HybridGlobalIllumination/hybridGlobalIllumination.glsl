#include "deferred.glsl"
#include "ReflectiveShadowMapping/reflectiveShadowMapping.glsl"

layout(std140) uniform hgiSamples
{
	int hgiSamplesCount;
	vec3 hgiSample[2000];
};

uniform sampler2D directLightMap;
uniform float ssdoRadius;

uniform float ssdoSamplingScale;

uniform int temporalFilterEnabled;

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

/*
 * Indirect Illumination Calculation
 * Thanks to: http://ericpolman.com/reflective-shadow-maps-part-2-the-implementation/
*/

vec3 CalcViewHGIIndirectDiffuseLight (in vec3 viewPosition, in vec3 viewNormal)
{
	if (viewPosition.z <= -cameraZLimits.y) {
		return vec3 (0);
	}

	// Compute fragment world position and world normal
	vec3 lightViewSpacePos = vec3 (lightViewMatrix * inverseViewMatrix * vec4 (viewPosition, 1.0));
	vec3 lightViewSpaceNormal = vec3 (lightViewMatrix * vec4 (normalMatrix * inverseNormalWorldMatrix * viewNormal, 0.0));

	vec3 indirectColor = vec3 (0.0);

	// vec4 lightSpacePos = lightProjectionMatrix * vec4 (lightViewSpacePos, 1.0);
	// vec3 rsmProjCoords = (lightSpacePos.xyz / lightSpacePos.w) * 0.5 + 0.5;

	float randDistanceScale = temporalFilterEnabled == 0 ? 1
		: rand (viewPosition.xy + viewPosition.yz + viewPosition.xz);
	vec3 randomVec = rand3 (viewPosition) - rand3 (2 * viewPosition);

	vec3 tangent;

	if (temporalFilterEnabled == 1) {
		tangent = normalize (randomVec - viewNormal * dot (randomVec, viewNormal));
	} else {
		tangent = normalize (vec3 (1, 0, 0) - lightViewSpaceNormal * dot (vec3 (1, 0, 0), lightViewSpaceNormal));
	}

	vec3 bitangent = cross (lightViewSpaceNormal, tangent);
	mat3 tangentMatrix = mat3 (tangent, bitangent, lightViewSpaceNormal);

	int samplesCount = 0;

	for (int index = 0; index < hgiSamplesCount; index ++) {

		vec3 sample = tangentMatrix * hgiSample [index];
		sample = lightViewSpacePos + normalize (sample) * ssdoRadius
			+ sample * (rsmRadius - ssdoRadius);// * randDistanceScale;

		vec4 coords = lightProjectionMatrix * vec4 (sample, 1.0);
		coords.xyz = (coords.xyz / coords.w) * 0.5 + 0.5;

		bvec2 a = greaterThan(coords.xy, vec2(1.0, 1.0));
		bvec2 b = lessThan(coords.xy, vec2(0.0, 0.0));

		if (any(bvec2(any(a), any(b)))) {
			continue;
		}

		vec3 rsmLightViewSpacePos = texture2D (rsmPositionMap, coords.xy).xyz;

		float sampleDistance = distance (lightViewSpacePos, rsmLightViewSpacePos);

		if (sampleDistance < ssdoRadius || sampleDistance > rsmRadius) {
			continue;
		}

		vec3 rsmLightViewSpaceNormal = texture2D (rsmNormalMap, coords.xy).xyz;
		vec3 rsmFlux = texture2D (rsmFluxMap, coords.xy).xyz;

		vec3 result = rsmFlux *
			((max (0.0, dot (rsmLightViewSpaceNormal, lightViewSpacePos - rsmLightViewSpacePos))
				* max (0.0, dot (lightViewSpaceNormal, rsmLightViewSpacePos - lightViewSpacePos)))
			/ max (pow (length (lightViewSpacePos - rsmLightViewSpacePos), 2.0), 1.0));

		//if (dot (result, result) > 0) {
			++ samplesCount;
		//}

		indirectColor += result;
	}

	samplesCount = max (samplesCount, 1);

	return indirectColor / samplesCount;
}

vec3 CalcHGIIndirectDiffuseLight (in vec3 in_position, in vec3 in_normal)
{
	if (in_position.z <= -cameraZLimits.y) {
		return vec3 (0);
	}

	vec3 indirectColor = vec3 (0.0);

	float randDistanceScale = temporalFilterEnabled == 0 ? 1
		: rand (in_position.xy + in_position.yz + in_position.xz);
	vec3 randomVec = rand3 (in_position) - rand3 (2 * in_position);

	vec3 tangent;

	if (temporalFilterEnabled == 1) {
		tangent = normalize (randomVec - in_normal * dot (randomVec, in_normal));
	} else {
		tangent = normalize (vec3 (1, 0, 0) - in_normal * dot (vec3 (1, 0, 0), in_normal));
	}

	vec3 bitangent = cross (in_normal, tangent);
	mat3 tangentMatrix = mat3 (tangent, bitangent, in_normal);

	int samplesCount = 0;

	float samplingLod = log2 (1.0 / ssdoSamplingScale);

	for (int sampleIndex = 0; sampleIndex < hgiSamplesCount; sampleIndex ++) {
		vec3 sample = tangentMatrix * hgiSample [sampleIndex];

		sample = in_position + sample * (ssdoRadius + 2);// * randDistanceScale;

		vec4 offset = projectionMatrix * vec4 (sample, 1.0);
		offset.xyz = (offset.xyz / offset.w) * 0.5 + 0.5;

		bvec2 a = greaterThan(offset.xy, vec2(1.0, 1.0));
		bvec2 b = lessThan(offset.xy, vec2(0.0, 0.0));

		if (any(bvec2(any(a), any(b)))) {
			continue;
		}

		vec3 samplePos = textureLod (gPositionMap, offset.xy, samplingLod).xyz;

		float sampleDistance = distance (in_position, samplePos);

		if (sampleDistance > ssdoRadius + 2) {
			continue;
		}

		vec3 sampleNormal = textureLod (gNormalMap, offset.xy, samplingLod).xyz;
		vec3 sampleDiffuse = textureLod (directLightMap, offset.xy, samplingLod).xyz;

		sampleNormal = normalize (sampleNormal);

		float dist = distance (in_position, samplePos);

		vec3 dir = normalize (in_position - samplePos);

		vec3 sampleColor = ((sampleDiffuse * max (dot (dir, sampleNormal), 0.0) *
			max (dot (-dir, in_normal), 0.0)) / max (pow (dist, 2), 1.0));

		indirectColor += sampleColor;

		//if (dot (sampleColor, sampleColor) > 0) {
			++ samplesCount;
		//}
	}

	samplesCount = max (samplesCount, 1);

	indirectColor /= samplesCount;

	return indirectColor;
}
