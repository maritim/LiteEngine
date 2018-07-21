#version 330 core

layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

#define VERTEX_COUNT 3

in vec3 vert_position[];
in vec3 vert_normal[];
in vec2 vert_texcoord[];
in vec2 vert_lmTexcoord[];

out vec3 geom_position;
out vec3 geom_normal;
out vec2 geom_texcoord;
out vec2 geom_lmTexcoord;

void main ()
{
	/*
	 * Emit every vertex of the triangle, ony by one
	*/

	for (int index = 0; index < VERTEX_COUNT; ++index) {
		gl_Position = gl_in [index].gl_Position;

		geom_position = vert_position [index];
		geom_normal = vert_normal [index];
		geom_texcoord = vert_texcoord [index];
		geom_lmTexcoord = vert_lmTexcoord [index];

		EmitVertex ();
	}

	EndPrimitive ();
}