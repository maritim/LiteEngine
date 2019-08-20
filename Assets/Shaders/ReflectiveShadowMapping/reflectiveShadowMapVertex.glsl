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

out vec3 vert_position;
out vec3 vert_normal;
out vec2 vert_texcoord;

void main()
{
	/*
	 * Emit position for rasterizer
	*/

	gl_Position =  modelViewProjectionMatrix * vec4 (in_position, 1);

	/*
	 * Emit position on the world
	*/

	vert_position = vec3 (modelMatrix * vec4 (in_position, 1));
	vert_normal = normalMatrix * in_normal;

	vert_texcoord = in_texcoord;
}
