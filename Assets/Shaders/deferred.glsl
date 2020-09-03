uniform sampler2D gPositionMap;
uniform sampler2D gNormalMap;
uniform sampler2D gDiffuseMap;
uniform sampler2D gSpecularMap;
uniform sampler2D gEmissiveMap;
uniform sampler2D gDepthMap;

uniform sampler2D gTrPositionMap;
uniform sampler2D gTrNormalMap;
uniform sampler2D gTrDiffuseMap;
uniform sampler2D gTrSpecularMap;
uniform sampler2D gTrEmissiveMap;
uniform sampler2D gTrDepthMap;

uniform vec2 screenSize;

vec2 CalcTexCoord()
{
	return gl_FragCoord.xy / screenSize;
}
