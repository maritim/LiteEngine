#version 430
layout (local_size_x = 4, local_size_y = 4, local_size_z = 4) in;

uniform int SrcMipLevel;
uniform int DstMipRes;

uniform ivec3 volumeSize;
uniform sampler3D volumeTexture;

layout(binding = 0, rgba8) uniform writeonly image3D dstImageMip;

void main() 
{
	if (gl_GlobalInvocationID.x < DstMipRes
		&& gl_GlobalInvocationID.y < DstMipRes
		&& gl_GlobalInvocationID.z < DstMipRes) {

		ivec3 dstPos = ivec3(gl_GlobalInvocationID);
  
		vec4 voxelColor = texelFetch (volumeTexture, dstPos, SrcMipLevel);

		if (voxelColor.a > 0) {
			return;
		}
		
		vec3 resultColor = vec3 (0.0);
		float contributionCount = 0;

		for (int x = max (0, dstPos.x - 1); x <= min (dstPos.x + 1, DstMipRes - 1); x ++) {
			for (int z = max (0, dstPos.z - 1); z <= min (dstPos.z + 1, DstMipRes - 1); z ++) {
				vec4 voxelColor = texelFetch (volumeTexture, ivec3 (x, dstPos.y, z), SrcMipLevel);

				if (voxelColor.a == 0) {
					continue;
				}

				resultColor += voxelColor.rgb;
				contributionCount ++;
			}
		}

		resultColor = contributionCount != 0.0 ? resultColor.rgb / contributionCount : vec3 (0.0);

		imageStore (dstImageMip, dstPos, vec4(resultColor, 0.0));
	}
}