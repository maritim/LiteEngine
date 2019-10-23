#version 330 core

layout(location = 0) out vec3 out_color;

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

uniform vec2 screenSize;

layout(std140) uniform ssdoSamples
{
	int ssdoSamplesCount;
	vec3 ssdoSample[200];
};

uniform vec2 ssdoResolution;
uniform float ssdoRadius;
uniform float ssdoBias;
uniform float ssdoIndirectIntensity;

vec2 CalcTexCoord()
{
	return gl_FragCoord.xy / ssdoResolution;
}

#include "AmbientLight/ambientLight.glsl"

/*
 * Calculate  a vector that is orthogonal to u.
*/

vec3 Orthogonal(vec3 u)
{
	u = normalize(u);

	vec3 v = vec3(0.0, 1.0, 0.0);

	return abs(dot(u, v)) > 0.99 ? cross(u, vec3(0, 0, 1)) : cross(u, v);
}

vec3 CalcScreenSpaceDirectionalOcclusion (vec3 in_position, vec3 in_normal, vec3 in_diffuse, vec2 texCoord)
{
	vec3 occlusionColor = vec3 (0.0);
	float occlusion = 0.0;

	/*
	 * Compute noise scale to map the noise on the whole screen
	*/

	vec3 tangent = Orthogonal (in_normal);
	vec3 bitangent = cross (in_normal, tangent);
	mat3 tangentMatrix = mat3 (tangent, bitangent, in_normal);

	int samplesCount = 0;

	for (int sampleIndex = 0; sampleIndex < ssdoSamplesCount; ++ sampleIndex) {
		vec3 sample = tangentMatrix * ssdoSample [sampleIndex];
		sample = in_position + sample * ssdoRadius;

		vec4 offset = projectionMatrix * vec4 (sample, 1.0);
		offset.xyz /= offset.w;
		offset.xyz = offset.xyz * 0.5 + 0.5;

		if (offset.x < 0 || offset.x > 1 || offset.y < 0 || offset.y > 1) {
			continue;
		}

		vec3 samplePos = texture2D (gPositionMap, offset.xy).xyz;
		vec3 sampleNormal = texture2D (gNormalMap, offset.xy).xyz;
		vec3 sampleDiffuse = texture2D (gDiffuseMap, offset.xy).xyz;

		sampleNormal = normalize (sampleNormal);

		int visibility = samplePos.z >= sample.z + ssdoBias ? 1 : 0;

		float dist = distance (in_position, samplePos);

		vec3 dir = normalize (in_position - samplePos);
		vec3 sampleColor = (ssdoRadius * ssdoRadius) * ((sampleDiffuse * max (dot (dir, sampleNormal), 0.0) *
			max (dot (-dir, in_normal), 0.0)) / max (dist * dist, 1.0));
		occlusionColor += sampleColor;

		float rangeCheck = smoothstep (0.0, 1.0, ssdoRadius / abs (in_position.z - samplePos.z));
		occlusion += (samplePos.z >= sample.z + ssdoBias ? 1.0 : 0.0) * rangeCheck;

		++ samplesCount;
	}

	samplesCount = max (samplesCount, 1);

	occlusion = 1.0 - (occlusion / samplesCount);
	occlusionColor /= samplesCount;

	return min (occlusion + occlusionColor * ssdoIndirectIntensity, 1.0) * ambientLightColor * ambientLightIntensity;
}

void main()
{
	vec2 texCoord = CalcTexCoord();
	vec3 in_position = texture2D (gPositionMap, texCoord).xyz;
	vec3 in_normal = texture2D (gNormalMap, texCoord).xyz;
	vec3 in_diffuse = texture2D (gDiffuseMap, texCoord).xyz;

	in_normal = normalize(in_normal);

	out_color = CalcScreenSpaceDirectionalOcclusion (in_position, in_normal, in_diffuse, texCoord);
}
