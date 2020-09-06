#version 330

layout(location = 0) out vec3 out_color;

#include "deferred.glsl"
#include "IndirectLight/indirectLight.glsl"

vec3 CalcIndirectLight (vec3 in_diffuse)
{
	vec3 indirectDiffuseLight = CalcIndirectDiffuseLight ();

	return in_diffuse * indirectDiffuseLight;
}

void main()
{
	vec2 texCoord = CalcTexCoord();
	vec3 in_diffuse = textureLod (gDiffuseMap, texCoord, 0).xyz;

	out_color = CalcIndirectLight (in_diffuse);
}
