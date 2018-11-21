#version 330 core

layout(location = 0) in vec3 in_position;
layout(location = 1) in vec3 in_normal;
layout(location = 2) in vec2 in_texcoord;
layout(location = 3) in ivec4 in_bone_id;
layout(location = 4) in vec4 in_weights;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelViewMatrix;
uniform mat4 modelViewProjectionMatrix;
uniform mat3 normalMatrix;
uniform mat3 normalWorldMatrix;

uniform mat4 boneTransforms[253];

out vec3 vert_worldPosition;
out vec3 vert_worldNormal;
out vec2 vert_texcoord;

void main()
{
	mat4 boneTransform = boneTransforms[in_bone_id[0]] * in_weights[0];
	boneTransform += boneTransforms[in_bone_id[1]] * in_weights[1];
	boneTransform += boneTransforms[in_bone_id[2]] * in_weights[2];
	boneTransform += boneTransforms[in_bone_id[3]] * in_weights[3];

	vec4 localPosition = boneTransform * vec4 (in_position, 1);

	/*
	 * Emit position for rasterizer
	*/

	gl_Position = modelMatrix * localPosition;

	/*
	 * Emit position on the world
	*/

	vert_worldPosition = vec3 (modelMatrix * localPosition);


	vec4 localNormal = boneTransform * vec4 (in_normal, 0.0);
	vert_worldNormal = vec3 (modelMatrix * localNormal);

	vert_texcoord = in_texcoord;
}
