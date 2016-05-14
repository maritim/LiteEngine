#version 330 core

layout(location = 0) in vec3 in_position;
layout(location = 1) in vec3 in_normal;
layout(location = 2) in vec2 in_texcoord;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelViewMatrix;
uniform mat4 modelViewProjectionMatrix;
uniform mat3 normalMatrix;
uniform mat3 normalWorldMatrix;

out vec3 position;
out vec3 normal;
out vec2 texcoord;

void main()
{
	gl_Position = modelViewProjectionMatrix * vec4 (in_position, 1);

	position = vec3 (modelMatrix * vec4 (in_position, 1));
	normal = in_normal;

	texcoord = in_texcoord;
}
