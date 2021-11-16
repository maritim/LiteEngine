#include "AmbientLight/ambientLight.glsl"

uniform sampler2D indirectDiffuseMap;
uniform sampler2D indirectSpecularMap;

uniform int indirectSpecularEnabled;

vec3 CalcIndirectDiffuseLight ()
{
	vec2 texCoord = CalcTexCoord();

	float ambientOcclusion = 1.0f;

	if (ambientOcclusionEnabled == 1) {
		float in_ao = texture (ambientOcclusionMap, texCoord).x;

		ambientOcclusion = in_ao;
	}

	vec3 in_indirect = texture (indirectDiffuseMap, texCoord).xyz;

	return in_indirect * ambientOcclusion;
}

vec3 fresnelSchlick(float cosTheta, vec3 F0)
{
    return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
}

vec3 CalcIndirectSpecularLight (const vec3 in_position, const vec3 in_normal)
{
	if (indirectSpecularEnabled == 0) {
		return vec3 (0.0f);
	}

	vec2 texCoord = CalcTexCoord();

	float ambientOcclusion = 1.0f;

	if (ambientOcclusionEnabled == 1) {
		float in_ao = texture (ambientOcclusionMap, texCoord).x;

		ambientOcclusion = in_ao;
	}

	vec3 in_indirect = texture (indirectSpecularMap, texCoord).xyz;

	vec3 fresnel = fresnelSchlick(max(dot(in_normal, normalize(-in_position)), 0.0), vec3 (0.0f));

	return fresnel * in_indirect * ambientOcclusion;
}
