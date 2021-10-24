layout(std140) uniform ssdoSamples
{
	int ssdoSamplesCount;
	vec3 ssdoSample[500];
};

uniform float ssdoRadius;
uniform float ssdoBias;
uniform float ssdoSamplingScale;

uniform int ssdoTemporalFilter;

uniform sampler2D postProcessMap;

/*
 * Calculate  a vector that is orthogonal to u.
*/

vec3 Orthogonal(vec3 u)
{
	u = normalize(u);

	vec3 v = vec3(0.0, 1.0, 0.0);

	return abs(dot(u, v)) > 0.99 ? cross(u, vec3(0, 0, 1)) : cross(u, v);
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

vec3 CalcScreenSpaceDirectionalOcclusion (vec3 in_position, vec3 in_normal, vec2 texcoord)
{
	vec3 occlusionColor = vec3 (0.0);
	float occlusion = 0.0;

	float r = ssdoTemporalFilter <= 0 ? 1 : rand (in_position.xy + in_position.yz + in_position.xz);
	vec3 randomVec = rand3 (in_position) - rand3 (2 * in_position);

	vec3 tangent;

	if (ssdoTemporalFilter > 0) {
		tangent = normalize (randomVec - in_normal * dot (randomVec, in_normal));
	} else {
		tangent = normalize (vec3 (1, 0, 0) - in_normal * dot (vec3 (1, 0, 0), in_normal));
	}

	vec3 bitangent = cross (in_normal, tangent);
	mat3 tangentMatrix = mat3 (tangent, bitangent, in_normal);

	// int samplesCount = 0;

	float samplingLod = log2 (1.0 / ssdoSamplingScale);

	for (int sampleIndex = 0; sampleIndex < ssdoSamplesCount; ++ sampleIndex) {
		vec3 sample = tangentMatrix * ssdoSample [sampleIndex];

		sample = in_position + sample * ssdoRadius * r;

		vec4 offset = projectionMatrix * vec4 (sample, 1.0);
		offset.xyz /= offset.w;
		offset.xyz = offset.xyz * 0.5 + 0.5;

		bvec2 a = greaterThan(offset.xy, vec2(1.0, 1.0));
		bvec2 b = lessThan(offset.xy, vec2(0.0, 0.0));

		if (any(bvec2(any(a), any(b)))) {
			continue;
		}

		vec3 samplePos = textureLod (gPositionMap, offset.xy, samplingLod).xyz;

		float dist = distance (in_position, samplePos);
		float texcoordDist = distance (texcoord, offset.xy);

		// if (dist > ssdoRadius) {
		// 	continue;
		// }

		vec3 sampleNormal = textureLod (gNormalMap, offset.xy, samplingLod).xyz;
		vec3 sampleDiffuse = textureLod (postProcessMap, offset.xy, samplingLod).xyz;

		sampleNormal = normalize (sampleNormal);

		// int visibility = samplePos.z >= sample.z + ssdoBias ? 1 : 0;

		vec3 dir = normalize (in_position - samplePos);
		// vec3 sampleColor = (ssdoRadius * ssdoRadius) * (
		// 	(sampleDiffuse * max (dot (dir, sampleNormal), 0.0) *
		// 		max (dot (-dir, in_normal), 0.0)) / max (dist * dist, 1.0));

		vec3 sampleColor = (texcoordDist * texcoordDist) *
			sampleDiffuse *
			max (0.0, dot (sampleNormal, dir))
				* max (0.0, dot (in_normal, -dir))
			/ pow (length (dist), 2.0);

		occlusionColor += sampleColor;

		// float rangeCheck = smoothstep (0.0, 1.0, ssdoRadius / abs (in_position.z - samplePos.z));
		// occlusion += (samplePos.z >= sample.z + ssdoBias ? 1.0 : 0.0) * rangeCheck;

		// ++ samplesCount;
	}

	// occlusion = 1.0 - (occlusion / samplesCount);

	occlusionColor /= ssdoSamplesCount;

	return max (occlusionColor, vec3 (0.0));
}
