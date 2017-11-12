#version 430
layout (local_size_x = 4, local_size_y = 4, local_size_z = 4) in;

/*
 * Input volume texture and volume properites
*/

uniform sampler3D volumeTexture;
uniform vec3 minVertex;
uniform vec3 maxVertex;
uniform ivec3 volumeSize;

/*
 * Input shadow map volume and light space properties
*/

uniform sampler2D shadowMap;
uniform mat4 lightSpaceMatrix;

/*
 * Output voxel volume
*/

layout(binding = 0, rgba8) uniform writeonly image3D voxelVolume;

float GetPosComponentInWorld (float comp, float minValue, float maxValue, float domain)
{
	return minValue + (comp / domain) * (maxValue - minValue);
}

vec3 GetVoxelPosInWorld (vec3 voxelPos)
{
	vec3 worldPos;

	worldPos.x = GetPosComponentInWorld (voxelPos.x, minVertex.x, maxVertex.x, volumeSize.x);
	worldPos.y = GetPosComponentInWorld (voxelPos.y, minVertex.y, maxVertex.y, volumeSize.y);
	worldPos.z = GetPosComponentInWorld (voxelPos.z, minVertex.z, maxVertex.z, volumeSize.z);

	return worldPos;
}

/*
 * Shadow Calculation
 * Thanks to: https://learnopengl.com/#!Advanced-Lighting/Shadows/Shadow-Mapping
*/

bool IsInShadow (vec3 worldPos)
{
	/*
	 * Transform into light space
	*/

	vec4 lightSpacePos = lightSpaceMatrix * vec4 (worldPos, 1.0);	

    /*
     * Perform perspective divide
    */

    vec3 projCoords = lightSpacePos.xyz / lightSpacePos.w;

	if(projCoords.z > 1.0)
        return false;

    /*
     * Transform to [0,1] range
    */

    projCoords = projCoords * 0.5 + 0.5;
    
    /*
     * Get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    */

    float closestDepth = texture (shadowMap, projCoords.xy).r; 

    /*
     * Get depth of current fragment from light's perspective
    */

    float currentDepth = projCoords.z;

    return currentDepth > closestDepth;	
}

void main() 
{
	if (gl_GlobalInvocationID.x < volumeSize.x
		&& gl_GlobalInvocationID.y < volumeSize.y
		&& gl_GlobalInvocationID.z < volumeSize.z) {

		/*
		 * Get voxel position in 3D texture
		*/

		ivec3 voxelPos = ivec3(gl_GlobalInvocationID);

		/*
		 * Extract voxel color
		*/

		vec4 voxelColor = texelFetch(volumeTexture, voxelPos, 0);

		if (voxelColor.a == 0.0)  {
			return;
		}

		/*
		 * Compute voxel position in world space
		*/

		vec3 voxelWorldPos = GetVoxelPosInWorld (vec3 (voxelPos));
		
		/*
		 * Do nothing is the voxel is not in the shadow
		*/

		if (!IsInShadow (voxelWorldPos)) {
			// imageStore
			return;
		}

		/*
		 * Store shadow color into voxel
		*/

		vec3 shadowColor = vec3 (0.0);

		imageStore(voxelVolume, voxelPos, vec4 (shadowColor, 1.0));		
	}
}