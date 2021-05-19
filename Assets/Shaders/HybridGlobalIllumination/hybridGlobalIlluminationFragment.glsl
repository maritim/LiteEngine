#version 330

layout(location = 0) out vec3 out_color;

#include "deferred.glsl"

uniform sampler2D directLightMap;

uniform sampler2D indirectDiffuseMap;
uniform sampler2D indirectSpecularMap;
uniform sampler2D ambientOcclusionMap;

uniform int indirectSpecularEnabled;
uniform int aoEnabled;

vec3 CalcIndirectSpecularLight (const in vec2 texCoord)
{
	if (indirectSpecularEnabled == 0) {
		return vec3 (0.0f);
	}

	return texture (indirectSpecularMap, texCoord).xyz;
}

float CalcAmbientOcclusion (const in vec2 texCoord)
{
	if (aoEnabled == 0) {
		return 1.0f;
	}

	return texture (ambientOcclusionMap, texCoord).x;
}

vec3 CalcIndirectLight (vec3 in_diffuse, vec3 in_direct, vec3 in_specular)
{
	vec2 texCoord = CalcTexCoord();

	vec3 indirectDiffuseLight = texture (indirectDiffuseMap, texCoord).xyz;
	vec3 indirectSpecularLight = CalcIndirectSpecularLight (texCoord);
	float ambientOcclusion = CalcAmbientOcclusion (texCoord);

	return in_direct + (in_diffuse * indirectDiffuseLight
		+ in_specular * indirectSpecularLight) * ambientOcclusion;
}

void main()
{
	vec2 texCoord = CalcTexCoord();
	vec3 in_diffuse = textureLod (gDiffuseMap, texCoord, 0).xyz;
	vec3 in_direct = texture (directLightMap, texCoord).xyz;
	vec3 in_specular = textureLod (gSpecularMap, texCoord, 0).xyz;

	out_color = CalcIndirectLight (in_diffuse, in_direct, in_specular);
}
