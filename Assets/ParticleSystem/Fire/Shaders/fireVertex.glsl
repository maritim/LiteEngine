#version 330

layout(location = 0) in vec3 in_position;
layout(location = 1) in vec3 in_normal;
layout(location = 2) in vec2 in_texcoord;
layout(location = 3) in mat4 in_modelMatrix;
layout(location = 7) in vec4 in_texcoordOffsets;
layout(location = 8) in vec4 in_blendingAndScale;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelViewMatrix;
uniform mat4 projectionMatrix;
uniform mat4 modelViewProjectionMatrix;
uniform mat3 normalMatrix;
uniform mat3 normalWorldMatrix;

uniform float atlasAreaScale;

out vec3 position;
out vec3 normal;
out vec2 texcoordCurr;
out vec2 texcoordNext;
out float blending;

void main()
{
	mat4 modelView = viewMatrix * in_modelMatrix;
	vec3 scale = in_blendingAndScale.xyz;

	// // Column 0
	modelView[0][0] = scale.x; // 0
	modelView[0][1] = 0; // 1
	modelView[0][2] = 0; // 2
	 
	// Column 1:
	modelView[1][0] = 0; // 4
	modelView[1][1] = scale.y; // 5
	modelView[1][2] = 0; // 6
	 
	// Column 2:
	modelView[2][0] = 0; // 8
	modelView[2][1] = 0; // 9
	modelView[2][2] = scale.z; // 10

	if (modelView [0][0] != 29) {
		gl_Position = vec4 (in_position, 1);
	}else {
		gl_Position = projectionMatrix * modelView * vec4 (in_position, 1);
	}
	
	texcoordCurr = in_texcoordOffsets.xy + in_texcoord * atlasAreaScale;
	texcoordNext = in_texcoordOffsets.zw + in_texcoord * atlasAreaScale;

	blending = in_blendingAndScale.w;
}