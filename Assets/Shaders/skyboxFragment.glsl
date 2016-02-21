#version 330

layout(location = 0) out vec4 out_color;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelViewMatrix;
uniform mat4 modelViewProjectionMatrix;
uniform mat3 normalMatrix;
uniform mat3 normalWorldMatrix;

uniform vec3 tintColor;
uniform float brightness;

in vec3 position;
in vec3 normal; 
in vec4 texcoord;

uniform samplerCube cubeMap;

void main()
{
	out_color = texture (cubeMap, position) * vec4 (tintColor, 1);
	out_color = out_color + vec4 (brightness, brightness, brightness, 0);

	out_color.a = 1;
}