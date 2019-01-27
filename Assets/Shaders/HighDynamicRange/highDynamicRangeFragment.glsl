#version 330 core

layout(location = 0) out vec3 out_color;

uniform sampler2D gPositionMap;
uniform sampler2D gNormalMap;
uniform sampler2D gDiffuseMap;
uniform sampler2D gSpecularMap;
uniform sampler2D lightAccumulationMap;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelViewMatrix;
uniform mat4 projectionMatrix;
uniform mat4 modelViewProjectionMatrix;
uniform mat3 normalMatrix;
uniform mat3 normalWorldMatrix;

uniform vec3 cameraPosition;

uniform vec2 screenSize;

uniform float exposure;

vec2 CalcTexCoord()
{
	return gl_FragCoord.xy / screenSize;
}

vec3 ReinhardToneMapping (const vec3 color)
{
	return color / (color + vec3 (1.0));
}

vec3 ExposureToneMapping (vec3 color)
{
	vec3 ldrColor = vec3 (1.0) - exp (-color * exposure);

	return ldrColor;
}

vec3 CalcHighDynamicRange (vec3 in_diffuse)
{
	return ExposureToneMapping (in_diffuse);
}

void main ()
{
	vec2 texCoord = CalcTexCoord();
	vec3 in_diffuse = texture2D (lightAccumulationMap, texCoord).xyz;

	out_color = CalcHighDynamicRange (in_diffuse);
}
