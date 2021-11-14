#version 330 core

layout(location = 0) out vec3 out_color;

uniform sampler2D gPositionMap;
uniform sampler2D gNormalMap;
uniform sampler2D gDiffuseMap;
uniform sampler2D gSpecularMap;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelViewMatrix;
uniform mat4 projectionMatrix;
uniform mat4 modelViewProjectionMatrix;
uniform mat3 normalMatrix;
uniform mat3 normalWorldMatrix;

uniform vec3 cameraPosition;

uniform vec2 screenSize;

uniform float bloomIntensity;

uniform sampler2D blurMap;
uniform sampler2D postProcessMap;

vec2 CalcTexCoord()
{
	return gl_FragCoord.xy / screenSize;
}

void main ()
{
	vec2 texCoord = CalcTexCoord();
	vec3 in_diffuse = texture (postProcessMap, texCoord).xyz;
	vec3 in_blur = texture (blurMap, texCoord).xyz;

	out_color = in_diffuse + in_blur * bloomIntensity;
}
