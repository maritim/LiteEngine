#version 330 core

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
out vec2 texcoord;

void main()
{
	gl_Position = projectionMatrix * viewMatrix * in_modelMatrix * vec4 (in_position, 1);

	normal = normalWorldMatrix * in_normal;
	texcoord = in_texcoord;
}