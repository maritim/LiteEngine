#version 330 core

#extension GL_ARB_shader_image_size : enable

in vec2 vs_RayCoordinates;

uniform sampler3D volumeTexture;

layout(location = 0) out vec4 outputColor;

void main ()
{
	vec3 oZPosition = vec3 (vs_RayCoordinates.x, vs_RayCoordinates.y, 0);

	vec4 fragColor = texture(volumeTexture, vec3 (0, 0, 0)); //oZPosition);

	outputColor = fragColor;
	// outputColor = vec4 (1.0, 0, 1.0, 1.0);
}

// uniform sampler3D VolumeTexture;
// uniform mat4 ModelViewProjectionInverse;
// uniform vec3 VolumeDimensions;
// uniform float StepSize;
// uniform float AlphaScale;
// uniform float AlphaBias;

// vec3 RayAABBTest(vec3 rayOrigin, vec3 rayDir, vec3 aabbMin, vec3 aabbMax)
// {
// 	float tMin, tMax;

// 	// Project ray through aabb
// 	vec3 invRayDir = 1.0 / rayDir;
// 	vec3 t1 = (aabbMin - rayOrigin) * invRayDir;
// 	vec3 t2 = (aabbMax - rayOrigin) * invRayDir;

// 	vec3 tmin = min(t1, t2);
// 	vec3 tmax = max(t1, t2);

// 	tMin = max(tmin.x, max(tmin.y, tmin.z));
// 	tMax = min(min(99999.0, tmax.x), min(tmax.y, tmax.z));

//     if (tMin < 0.0) tMin = 0.0;

// 	vec3 result;
// 	result.x = (tMax > tMin) ? 1.0 : 0.0;
// 	result.y = tMin;
// 	result.z = tMax;
// 	return result;
// }

// void main()
// {
//     // Reverse y coordinate of ray coordinates.
//     vec2 rayCoords = vec2(vs_RayCoordinates.x, vs_RayCoordinates.y);
    
//     // Compute ray's origin and direction from pixel coordinate.
// 	vec4 ndc0 = vec4(rayCoords, -1.0, 1.0);
// 	vec4 ndc1 = vec4(rayCoords, -0.5, 1.0);
// 	vec4 world0 = ModelViewProjectionInverse * ndc0;
// 	vec4 world1 = ModelViewProjectionInverse * ndc1;
// 	world0 /= world0.w;
// 	world1 /= world1.w;

// 	vec3 rayOrigin = world0.xyz;
// 	vec3 rayDir = world1.xyz - world0.xyz;
// 	rayDir = normalize(rayDir);

//     // Check for ray components being parallel to axes (i.e. values of 0).
// 	const float epsilon = 0.00001;
// 	if (abs(rayDir.x) <= epsilon) rayDir.x = epsilon * sign(rayDir.x);
// 	if (abs(rayDir.y) <= epsilon) rayDir.y = epsilon * sign(rayDir.y);
// 	if (abs(rayDir.z) <= epsilon) rayDir.z = epsilon * sign(rayDir.z);

// 	// Calculate inverse of ray direction once.
// 	vec3 invRayDir = 1.0 / rayDir;
   
//     // Perform AABB test with volume.
// 	vec4 finalColor = vec4(0.4f);
// 	vec3 result = RayAABBTest(rayOrigin, rayDir, vec3(0.0), VolumeDimensions);
//     if (result.x != 0.0)
//     {		
//         // Extract out ray's start and end position.
//         float tMin = result.y;
//         float tMax = result.z;

// 		vec3 startPos = rayOrigin + rayDir * tMin;
// 		vec3 voxelPos = max(vec3(0.0), min(VolumeDimensions - vec3(1.0), floor(startPos)));

// 		// Traverse through voxels until ray exits volume.
// 		while (all(greaterThanEqual(voxelPos, vec3(0.0))) && all(lessThan(voxelPos, VolumeDimensions)))
// 		{
// 			// Sample 3D texture at current position.
// 			vec3 texCoords = voxelPos / VolumeDimensions;
// 			vec4 color = texture(VolumeTexture, texCoords);

// 			// Exit loop if a single sample has an alpha value greater than 0.
// 			if (color.a > 0.0)
// 			{
// 				finalColor = color;
// 				break;
// 			}

// 			// Move to next closest voxel along ray.
// 			vec3 t0 = (voxelPos - startPos) * invRayDir;
// 			vec3 t1 = (voxelPos + vec3(1.0) - startPos) * invRayDir;
// 			vec3 tmax = max(t0, t1);
// 			float t = min(tmax.x, min(tmax.y, tmax.z));
// 			if (tmax.x == t) voxelPos.x += sign(rayDir.x);
// 			else if (tmax.y == t) voxelPos.y += sign(rayDir.y);
// 			else if (tmax.z == t) voxelPos.z += sign(rayDir.z);
// 		}
//     }
    
// 	// Write final color to framebuffer.
// 	FragColor = finalColor;
// }