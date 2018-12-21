#version 330 core

layout(location = 0) out vec3 out_color;

uniform sampler2D gPositionMap;
uniform sampler2D gNormalMap;
uniform sampler2D gDiffuseMap;
uniform sampler2D gSpecularMap;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelViewMatrix;
uniform mat4 modelViewProjectionMatrix;
uniform mat3 normalMatrix;
uniform mat3 normalWorldMatrix;

uniform vec3 cameraPosition;

uniform vec3 ambientLightColor;

uniform vec2 screenSize;

uniform sampler2D ambientOcclusionMap;

vec2 CalcTexCoord()
{
	return gl_FragCoord.xy / screenSize;
}

vec3 CalcAmbientLight (vec3 in_diffuse, float in_ao)
{
	return in_diffuse * ambientLightColor * in_ao;
}

void main()
{
	vec2 texCoord = CalcTexCoord();
	vec3 in_diffuse = texture2D (gDiffuseMap, texCoord).xyz;
	float in_ao = texture2D (ambientOcclusionMap, texCoord).x;

	out_color = CalcAmbientLight (in_diffuse, in_ao);
}
