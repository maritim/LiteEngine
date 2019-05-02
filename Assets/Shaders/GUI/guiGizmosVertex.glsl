#version 330

layout(location = 0) in vec3 in_position;
layout(location = 1) in vec3 in_color;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelViewMatrix;
uniform mat4 projectionMatrix;
uniform mat4 modelViewProjectionMatrix;
uniform mat3 normalMatrix;
uniform mat3 normalWorldMatrix;

out vec3 color;

void main ()
{
	color = in_color;

	gl_Position = modelViewProjectionMatrix * vec4 (in_position, 1);
}
