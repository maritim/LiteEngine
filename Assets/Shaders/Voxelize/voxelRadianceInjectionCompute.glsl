#version 430
layout (local_size_x = 4, local_size_y = 4, local_size_z = 4) in;

// Compute-based mipmapping inspired by 

uniform sampler3D volumeTexture;
uniform vec3 minVertex;
uniform vec3 maxVertex;
uniform ivec3 volumeSize;

uniform sampler2D shadowMap;
uniform mat4 lightSpaceMatrix;

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

bool IsInShadow (vec4 lightSpacePos)
{
    // perform perspective divide
    vec3 projCoords = lightSpacePos.xyz / lightSpacePos.w;

	if(projCoords.z > 1.0)
        return false;

    // Transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;
    
    // Get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture (shadowMap, projCoords.xy).r; 

    // Get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;
    
	float bias = 0.002;

    return currentDepth - bias > closestDepth;	
}

void main() 
{
	if (gl_GlobalInvocationID.x < volumeSize.x
		&& gl_GlobalInvocationID.y < volumeSize.y
		&& gl_GlobalInvocationID.z < volumeSize.z) {

		ivec3 voxelPos = ivec3(gl_GlobalInvocationID);

		vec4 voxelColor = texelFetch(volumeTexture, voxelPos, 0);

		if (voxelColor.a == 0.0)  {
			return;
		}

		vec3 voxelWorldPos = GetVoxelPosInWorld (vec3 (voxelPos));

		vec4 lightSpacePos = lightSpaceMatrix * vec4 (voxelWorldPos, 1.0f);	
		
		if (!IsInShadow (lightSpacePos)) {
			return;
		}

		vec3 shadowColor = vec3 (0.0);

		imageStore(voxelVolume, voxelPos, vec4 (shadowColor, 1.0));		
	}
}