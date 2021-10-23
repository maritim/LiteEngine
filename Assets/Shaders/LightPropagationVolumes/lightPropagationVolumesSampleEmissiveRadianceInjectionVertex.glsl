#version 430 core

layout(location = 0) in vec3 in_position;
layout(location = 1) in vec3 in_normal;
layout(location = 2) in vec2 in_texcoord;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelViewMatrix;
uniform mat4 modelViewProjectionMatrix;
uniform mat3 normalMatrix;
uniform mat3 normalWorldMatrix;

uniform int vplsCount;

out vec3 vert_position;
out vec3 vert_normal;
out vec2 vert_texcoord;
out vec3 vert_worldPosition;
out vec3 vert_worldNormal;

void main()
{
	/*
	 * Emit position for rasterizer
	*/

	gl_Position = vec4 (in_position, 1);

	/*
	 * Emit position on the world
	*/

	vert_position = in_position;
	vert_normal = in_normal;
	vert_texcoord = in_texcoord;
}
