#version 330

layout(location = 0) in vec2 in_position;
layout(location = 1) in vec2 in_texcoord;
layout(location = 2) in vec4 in_color;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelViewMatrix;
uniform mat4 projectionMatrix;
uniform mat4 modelViewProjectionMatrix;
uniform mat3 normalMatrix;
uniform mat3 normalWorldMatrix;

out vec2 texcoord;
out vec4 color;

void main ()
{
	texcoord = in_texcoord;
	color = in_color;

	gl_Position = projectionMatrix * vec4 (in_position.xy, 0, 1);
}
