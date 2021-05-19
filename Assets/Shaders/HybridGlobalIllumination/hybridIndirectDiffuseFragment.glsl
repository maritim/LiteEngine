#version 330

layout(location = 0) out vec3 out_color;

#include "deferred.glsl"

uniform sampler2D rsmIndirectDiffuseMap;
uniform sampler2D ssdoIndirectDiffuseMap;

void main()
{
	vec2 texCoord = CalcTexCoord();

	vec3 rsmIndirectDiffuseLight = texture (rsmIndirectDiffuseMap, texCoord).xyz;
	vec3 ssdoIndirectDiffuseLight = texture (ssdoIndirectDiffuseMap, texCoord).xyz;

	out_color = rsmIndirectDiffuseLight + ssdoIndirectDiffuseLight;
}
