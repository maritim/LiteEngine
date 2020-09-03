#include "AmbientLight/ambientLight.glsl"

uniform sampler2D indirectDiffuseMap;
uniform sampler2D indirectSpecularMap;

vec3 CalcIndirectDiffuseLight ()
{
	vec2 texCoord = CalcTexCoord();

	float ambientOcclusion = 1.0f;

	if (ambientOcclusionEnabled == 1) {
		float in_ao = texture2D (ambientOcclusionMap, texCoord).x;

		ambientOcclusion = in_ao;
	}

	vec3 in_indirect = texture2D (indirectDiffuseMap, texCoord).xyz;

	return in_indirect * ambientOcclusion;
}

vec3 CalcIndirectSpecularLight ()
{
	vec2 texCoord = CalcTexCoord();

	float ambientOcclusion = 1.0f;

	if (ambientOcclusionEnabled == 1) {
		float in_ao = texture2D (ambientOcclusionMap, texCoord).x;

		ambientOcclusion = in_ao;
	}

	vec3 in_indirect = texture2D (indirectSpecularMap, texCoord).xyz;

	return in_indirect * ambientOcclusion;
}
