#version 330

layout(location = 0) out vec3 out_color;

uniform sampler2D indirectMap;

#include "deferred.glsl"

vec3 CalcIndirectLight (vec3 in_diffuse, vec3 in_indirect)
{
	return in_diffuse * in_indirect;
}

void main()
{
	vec2 texCoord = CalcTexCoord();
	vec3 in_diffuse = texture2D (gDiffuseMap, texCoord).xyz;
	vec3 in_indirect = texture2D (indirectMap, texCoord).xyz;

	out_color = CalcIndirectLight (in_diffuse, in_indirect);
}
