#version 330

layout(location = 0) out vec3 out_color;

#include "deferred.glsl"

uniform sampler2D directLightMap;

uniform sampler2D rsmIndirectDiffuseMap;
uniform sampler2D ssdoIndirectDiffuseMap;
uniform sampler2D indirectSpecularMap;

vec3 CalcIndirectLight (vec3 in_diffuse, vec3 in_direct, vec3 in_specular)
{
	vec2 texCoord = CalcTexCoord();

	vec3 rsmIndirectDiffuseLight = texture (rsmIndirectDiffuseMap, texCoord).xyz;
	vec3 ssdoIndirectDiffuseLight = texture (ssdoIndirectDiffuseMap, texCoord).xyz;
	vec3 indirectSpecularLight = texture (indirectSpecularMap, texCoord).xyz;

	return in_direct + in_diffuse * (rsmIndirectDiffuseLight + ssdoIndirectDiffuseLight)
		+ in_specular * indirectSpecularLight;
}

void main()
{
	vec2 texCoord = CalcTexCoord();
	vec3 in_diffuse = textureLod (gDiffuseMap, texCoord, 0).xyz;
	vec3 in_direct = texture (directLightMap, texCoord).xyz;
	vec3 in_specular = textureLod (gSpecularMap, texCoord, 0).xyz;

	out_color = CalcIndirectLight (in_diffuse, in_direct, in_specular);
}
