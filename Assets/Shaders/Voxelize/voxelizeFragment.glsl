#version 420 core

#extension GL_ARB_shader_image_size : enable

#define PIXEL_DIAGONAL 2000

coherent uniform layout (binding = 0, rgba8) writeonly image3D volumeTexture;
layout (location = 0) out vec4 fragColor;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelViewMatrix;
uniform mat4 modelViewProjectionMatrix;
uniform mat3 normalMatrix;
uniform mat3 normalWorldMatrix;

uniform vec4 MaterialDiffuse;

uniform sampler2D DiffuseMap;

uniform vec3 cameraPosition;

uniform vec3 sceneAmbient;

uniform vec3 minPosition;
uniform vec3 maxPosition;

uniform vec3 volumeSize;

in vec3 geom_worldPosition;
in vec3 geom_worldNormal;
in vec2 geom_texcoord;
in mat3 geom_swizzleMatrixInv;

// vec3 PhongModel(vec3 diffuseMap)
// {
// 	lightDir = normalize(vec3 (-0.5, -0.5, -0.5));
 
//     float sDotN = max(dot(lightDir, normal), 0.0);
//     vec3 diffuse = textureColor * sDotN;

//     return ambient + diffuse;
// }

float GetInterpolatedPosition (float x, float minValue, float maxValue, int domain)
{
	return ((x - minValue) / maxValue) * domain;
}

vec3 GetPositionInTexture3D (vec3 position)
{
	vec3 resultCoords;

	resultCoords.x = GetInterpolatedPosition (position.x, minPosition.x, maxPosition.x, textureSize.x);
	resultCoords.y = GetInterpolatedPosition (position.y, minPosition.y, maxPosition.y, textureSize.y);
	resultCoords.z = GetInterpolatedPosition (position.z, minPosition.z, maxPosition.z, textureSize.z);

	return resultCoords;
}

void main()
{
	/*
	 * Get color of all used texture maps
	*/

	vec3 diffuseMap = vec3 (MaterialDiffuse * texture2D (DiffuseMap, geom_texcoord.xy));

	vec3 fragmentColor = diffuseMap;

	/*
	 * Calculate the position in texture 3D
	*/
	
	ivec3 textureSize = imageSize (volumeTexture);

	vec3 coords = geom_swizzleMatrixInv * geom_worldPosition;
	coords = GetPositionInTexture3D (coords);

	/*
	 * Save in texture
	*/

	imageStore(volumeTexture, ivec3(coords), vec4(fragmentColor, 1.0));
	memoryBarrier ();
}