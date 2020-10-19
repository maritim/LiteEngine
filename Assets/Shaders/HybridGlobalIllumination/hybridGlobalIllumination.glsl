#include "deferred.glsl"
#include "ReflectiveShadowMapping/reflectiveShadowMapping.glsl"

layout(std140) uniform hgiSamples
{
	int hgiSamplesCount;
	vec3 hgiSample[2000];
};

uniform sampler2D directLightMap;
uniform int ssdoSampleCount;
uniform float ssdoRadius;

/*
 * Indirect Illumination Calculation
 * Thanks to: http://ericpolman.com/reflective-shadow-maps-part-2-the-implementation/
*/

vec3 CalcViewHGIIndirectDiffuseLight (in vec3 viewPosition, in vec3 viewNormal)
{
	// Compute fragment world position and world normal
	vec3 lightViewSpacePos = vec3 (lightViewMatrix * inverseViewMatrix * vec4 (viewPosition, 1.0));
	vec3 lightViewSpaceNormal = vec3 (lightViewMatrix * vec4 (normalMatrix * inverseNormalWorldMatrix * viewNormal, 0.0));

	vec3 indirectColor = vec3 (0.0);

	// vec4 lightSpacePos = lightProjectionMatrix * vec4 (lightViewSpacePos, 1.0);
	// vec3 rsmProjCoords = (lightSpacePos.xyz / lightSpacePos.w) * 0.5 + 0.5;

	vec3 tangent = normalize (vec3 (1, 0, 0) - lightViewSpaceNormal * dot (vec3 (1, 0, 0), lightViewSpaceNormal));
	vec3 bitangent = cross (lightViewSpaceNormal, tangent);
	mat3 tangentMatrix = mat3 (tangent, bitangent, lightViewSpaceNormal);

	int samplesCount = 0;

	for (int index = 0; index < hgiSamplesCount; index ++) {

		vec3 sample = tangentMatrix * hgiSample [index];
		sample = lightViewSpacePos + normalize (sample) * ssdoRadius + sample * (rsmRadius - ssdoRadius);

		vec4 coords = lightProjectionMatrix * vec4 (sample, 1.0);
		coords.xyz = (coords.xyz / coords.w) * 0.5 + 0.5;

		if (coords.x < 0 || coords.x > 1 || coords.y < 0 || coords.y > 1) {
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
			/ pow (length (lightViewSpacePos - rsmLightViewSpacePos), 4.0));

		if (dot (result, result) > 0) {
			++ samplesCount;
		}

		indirectColor += result;
	}

	samplesCount = max (samplesCount, 1);

	return indirectColor;// / samplesCount;
}

vec3 CalcHGIIndirectDiffuseLight (in vec3 in_position, in vec3 in_normal)
{
	vec3 occlusionColor = vec3 (0.0);

	vec3 tangent = normalize (vec3 (1, 0, 0) - in_normal * dot (vec3 (1, 0, 0), in_normal));
	vec3 bitangent = cross (in_normal, tangent);
	mat3 tangentMatrix = mat3 (tangent, bitangent, in_normal);

	int samplesCount = 0;

	int step = hgiSamplesCount / ssdoSampleCount;

	for (int sampleIndex = 0; sampleIndex < hgiSamplesCount; sampleIndex += step) {
		vec3 sample = tangentMatrix * hgiSample [sampleIndex];

		sample = in_position + sample * (ssdoRadius + 2);

		vec4 offset = projectionMatrix * vec4 (sample, 1.0);
		offset.xyz = (offset.xyz / offset.w) * 0.5 + 0.5;

		if (offset.x < 0 || offset.x > 1 || offset.y < 0 || offset.y > 1) {
			continue;
		}

		vec3 samplePos = textureLod (gPositionMap, offset.xy, 0).xyz;

		float sampleDistance = distance (in_position, samplePos);

		if (sampleDistance > ssdoRadius + 2) {
			continue;
		}

		vec3 sampleNormal = textureLod (gNormalMap, offset.xy, 0).xyz;
		vec3 sampleDiffuse = texture2D (directLightMap, offset.xy).xyz;

		sampleNormal = normalize (sampleNormal);

		float dist = distance (in_position, samplePos);

		vec3 dir = normalize (in_position - samplePos);

		vec3 sampleColor = ((sampleDiffuse * max (dot (dir, sampleNormal), 0.0) *
			max (dot (-dir, in_normal), 0.0)) / max (pow (dist, 2), 1.0));

		occlusionColor += sampleColor;

		if (dot (sampleColor, sampleColor) > 0) {
			++ samplesCount;
		}
	}

	samplesCount = max (samplesCount, 1);

	occlusionColor /= samplesCount;

	if (samplesCount < ssdoSampleCount / 20) {
		return vec3 (0);
	}

	return occlusionColor;
}
