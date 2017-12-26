#version 330

layout(location = 0) in vec3 in_position;
layout(location = 1) in vec3 in_normal;
layout(location = 2) in vec2 in_texcoord;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelViewMatrix;
uniform mat4 projectionMatrix;
uniform mat4 modelViewProjectionMatrix;
uniform mat3 normalMatrix;
uniform mat3 normalWorldMatrix;

uniform vec3 cameraPosition;

out vec3 position;
out vec3 normal;
out vec2 texcoord;

void main()
{
	mat4 view = viewMatrix;
	view [3][0] = 0;
	view [3][1] = 0;
	view [3][2] = 0;

	gl_Position = projectionMatrix * view * modelMatrix * vec4 (in_position, 1);

	position = in_position;
}