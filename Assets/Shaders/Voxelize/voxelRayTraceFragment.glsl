#version 330 core

in vec2 geom_RayCoordinates;

uniform vec3 cameraPosition;

uniform sampler3D volumeTexture;

uniform vec3 minVertex;
uniform vec3 maxVertex;
uniform ivec3 volumeSize;
uniform int volumeMipmapLevel;


uniform mat4 inverseViewProjectionMatrix;

layout(location = 0) out vec4 outputColor;

vec3 RayBoundingBoxTest(vec3 rayOrigin, vec3 rayDir, vec3 vertexMin, vec3 vertexMax)
{
	float tMin, tMax;

	// Project ray through aabb
	vec3 invRayDir = 1.0 / rayDir;
	vec3 t1 = (vertexMin - rayOrigin) * invRayDir;
	vec3 t2 = (vertexMax - rayOrigin) * invRayDir;

	vec3 tmin = min(t1, t2);
	vec3 tmax = max(t1, t2);

	tMin = max(tmin.x, max(tmin.y, tmin.z));
	tMax = min(min(99999.0, tmax.x), min(tmax.y, tmax.z));

	if (tMin < 0.0) tMin = 0.0;

	vec3 result;
	result.x = (tMax > tMin) ? 1.0 : 0.0;
	result.y = tMin;
	result.z = tMax;
	return result;
}

float GetInterpolatedComp (float comp, float minValue, float maxValue, float domain)
{
	return ((comp - minValue) / (maxValue - minValue)) * domain;
}

vec3 GetRayOriginInVolume (vec3 worldRayOrigin, ivec3 volumeSize)
{
	vec3 rayOriginInVolume;

	rayOriginInVolume.x = GetInterpolatedComp (worldRayOrigin.x, minVertex.x, maxVertex.x, volumeSize.x);
	rayOriginInVolume.y = GetInterpolatedComp (worldRayOrigin.y, minVertex.y, maxVertex.y, volumeSize.y);
	rayOriginInVolume.z = GetInterpolatedComp (worldRayOrigin.z, minVertex.z, maxVertex.z, volumeSize.z);

	return rayOriginInVolume;
}

ivec3 GetMipmapVolumeSize (ivec3 volumeSize, int mipmapLevel)
{
	ivec3 mipmapVolumeSize;

	mipmapVolumeSize.x = volumeSize.x >> mipmapLevel;
	mipmapVolumeSize.y = volumeSize.y >> mipmapLevel;
	mipmapVolumeSize.z = volumeSize.z >> mipmapLevel;

	return mipmapVolumeSize;
}

void main ()
{
	/*
	 * Recalculate volume size based on mipmap level
	*/

	ivec3 mipmapVolumeSize = GetMipmapVolumeSize (volumeSize, volumeMipmapLevel);

    /*
	 * Compute ray's origin and direction from pixel coordinate.
	*/

 	vec4 screenSpaceRayCoords1 = vec4 (geom_RayCoordinates, -1.0, 1.0);
	vec4 screenSpaceRayCoords2 = vec4 (geom_RayCoordinates, -0.5, 1.0);
 	vec4 worldRayCoords1 = inverseViewProjectionMatrix * screenSpaceRayCoords1;
	vec4 worldRayCoords2 = inverseViewProjectionMatrix * screenSpaceRayCoords2;
 	worldRayCoords1 /= worldRayCoords1.w;
	worldRayCoords2 /= worldRayCoords2.w;

 	vec3 rayOrigin = worldRayCoords1.xyz;
 	vec3 rayDir = worldRayCoords2.xyz - worldRayCoords1.xyz;
 	rayDir = normalize(rayDir);

 	rayOrigin = GetRayOriginInVolume (rayOrigin, mipmapVolumeSize);

    /*
	 * Check for ray components being parallel to axes (i.e. values of 0).
	*/

 	const float epsilon = 0.00001;
 	if (abs(rayDir.x) <= epsilon) rayDir.x = epsilon * sign(rayDir.x);
 	if (abs(rayDir.y) <= epsilon) rayDir.y = epsilon * sign(rayDir.y);
 	if (abs(rayDir.z) <= epsilon) rayDir.z = epsilon * sign(rayDir.z);

	/*
	 * Perform AABB test with volume.
	*/

 	vec3 result = RayBoundingBoxTest (rayOrigin, rayDir, vec3 (0.0), vec3 (mipmapVolumeSize));

	if (result.x == 0.0) {
		discard;
	}

 	vec4 finalColor = vec4((rayDir + vec3 (1.0)) / 2.0 , 1.0);

	/*
	 * Extract out ray's start and end position.
	*/

	float tMin = result.y;
	float tMax = result.z;

	vec3 startPos = rayOrigin + rayDir * tMin;
	vec3 voxelPos = max(vec3(0.0), min(mipmapVolumeSize - vec3(1.0), floor(startPos)));

	/*
	 * Traverse through voxels until ray exits volume.
	*/

 	// Calculate inverse of ray direction once.
 	vec3 invRayDir = 1.0 / rayDir;

	while (all(greaterThanEqual(voxelPos, vec3(0.0))) && all(lessThan(voxelPos, mipmapVolumeSize)))
	{
		// Sample 3D texture at current position.
		vec3 texCoords = voxelPos / mipmapVolumeSize;
		vec4 color = textureLod (volumeTexture, texCoords, volumeMipmapLevel);


		// Exit loop if a single sample has an alpha value greater than 0.
		if (color.a > 0.0) {
 			finalColor = vec4 (color.xyz, 1.0);
 			break;
		}

		// Move to next closest voxel along ray.
		vec3 t0 = (voxelPos - startPos) * invRayDir;
		vec3 t1 = (voxelPos + vec3(1.0) - startPos) * invRayDir;
		vec3 tmax = max(t0, t1);
		float t = min(tmax.x, min(tmax.y, tmax.z));
		if (tmax.x == t) voxelPos.x += sign(rayDir.x);
		else if (tmax.y == t) voxelPos.y += sign(rayDir.y);
		else if (tmax.z == t) voxelPos.z += sign(rayDir.z);
	}
    
 	/*
	 * Write final color to framebuffer.
	*/

 	outputColor = finalColor;
}

