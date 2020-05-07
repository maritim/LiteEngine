uniform sampler2D gPositionMap;
uniform sampler2D gNormalMap;
uniform sampler2D gDiffuseMap;
uniform sampler2D gSpecularMap;
uniform sampler2D gEmissiveMap;
uniform sampler2D gDepthMap;

uniform vec2 screenSize;

vec2 CalcTexCoord()
{
	return gl_FragCoord.xy / screenSize;
}
