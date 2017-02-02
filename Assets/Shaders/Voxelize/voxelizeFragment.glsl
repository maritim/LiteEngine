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

void main()
{
	/*
	 * Get color of all used texture maps
	*/

	vec3 diffuseMap = vec3 (MaterialDiffuse * texture2D (DiffuseMap, geom_texcoord.xy));

	ivec2 viewportSize = imageSize(volumeTexture).xy;

	// vec2 bboxMin = floor((gs_BBox.xy * 0.5 + 0.5) * viewportSize);
	// vec2 bboxMax = ceil((gs_BBox.zw * 0.5 + 0.5) * viewportSize);
	
	// if (all(greaterThanEqual(gl_FragCoord.xy, bboxMin)) && all(lessThanEqual(gl_FragCoord.xy, bboxMax))) {
	ivec3 textureSize = imageSize (volumeTexture);

	vec3 coords = geom_swizzleMatrixInv * gl_FragCoord.xyz;
	coords.x = GetInterpolatedPosition (coords.x, minPosition.x, maxPosition.x, textureSize.x);
	coords.y = GetInterpolatedPosition (coords.y, minPosition.y, maxPosition.y, textureSize.y);
	coords.z = GetInterpolatedPosition (coords.z, minPosition.z, maxPosition.z, textureSize.z);

	vec3 fragmentColor = diffuseMap;

	imageStore(volumeTexture, ivec3(coords), vec4(fragmentColor, 1.0));
	memoryBarrier ();

	// imageStore(volumeTexture, ivec3(0, 0, 0), vec4(1.0, 1.0, 1.0, 1.0));
	// imageStore(volumeTexture, ivec3(255, 254, 0), vec4(0.0, 0.0, 0.0, 1.0));
	// imageStore(volumeTexture, ivec3(254, 254, 0), vec4(0.0, 0.0, 0.0, 1.0));
	// imageStore(volumeTexture, ivec3(254, 255, 0), vec4(0.0, 0.0, 0.0, 1.0));

	// }
	// else{
	//     discard;
	// }	
}