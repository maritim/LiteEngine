#version 330 core

layout(location = 0) in vec2 in_position;
layout(location = 1) in vec2 in_texcoord;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelViewMatrix;
uniform mat4 modelViewProjectionMatrix;
uniform mat3 normalMatrix;
uniform mat3 normalWorldMatrix;

out vec2 texcoord;

void main()
{
	gl_Position = modelMatrix * vec4 (in_position, 0, 1);

	texcoord = in_texcoord;
}