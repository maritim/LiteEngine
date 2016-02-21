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

uniform float atlasAreaScale;
uniform vec2 atlasAreaOffset;
uniform vec2 atlasNextAreaOffset;

out vec3 position;
out vec3 normal;
out vec2 texcoordCurr;
out vec2 texcoordNext;

void main()
{
	mat4 modelView = mat4 (modelViewMatrix);

	// // Column 0
	modelView[0][0] = 1; // 0
	modelView[0][1] = 0; // 1
	modelView[0][2] = 0; // 2
	 
	// Column 1:
	// modelView[1][0] = 0; // 4
	// modelView[1][1] = 1; // 5
	// modelView[1][2] = 0; // 6
	 
	// Column 2:
	modelView[2][0] = 0; // 8
	modelView[2][1] = 0; // 9
	modelView[2][2] = 1; // 10

	gl_Position = projectionMatrix * modelView * vec4 (in_position, 1);

	position = vec3 (modelMatrix * vec4 (in_position, 1));
	normal = normalWorldMatrix * in_normal;
	
	texcoordCurr = atlasAreaOffset + in_texcoord * atlasAreaScale;
	texcoordNext = atlasNextAreaOffset + in_texcoord * atlasAreaScale;
}