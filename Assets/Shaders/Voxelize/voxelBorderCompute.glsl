#version 430
layout (local_size_x = 4, local_size_y = 4, local_size_z = 4) in;

// Compute-based mipmapping inspired by 

/*  Workgroup Size Stats

1^3 167.00 ms
2^3  21.23 ms
4^3   3.77 ms
8^3   4.02 ms  */

//uniform int SrcMipLevel;
//uniform int DstMipRes;

uniform ivec3 volumeSize;
uniform sampler3D volumeTexture;

layout(binding = 0, rgba8) uniform writeonly image3D dstImageMip;

void main() 
{
	if (gl_GlobalInvocationID.x < volumeSize.x
		&& gl_GlobalInvocationID.y < volumeSize.y
		&& gl_GlobalInvocationID.z < volumeSize.z) {

		ivec3 dstPos = ivec3(gl_GlobalInvocationID);
  
		vec4 voxelColor = texelFetch (volumeTexture, dstPos, 0);

		if (voxelColor.a == 0) {
			return;
		}
		
		imageStore (dstImageMip, dstPos, vec4(voxelColor.xyz, 1.0));

		//vec4 resultColor = vec4 (0.0);
		//float contributionCount = 0;

		//for (int x = max (0, dstPos.x - 1); x <= min (dstPos.x + 1, DstMipRes - 1); x ++) {
		//	for (int z = max (0, dstPos.z - 1); z <= min (dstPos.z + 1, DstMipRes - 1); z ++) {
		//		vec4 voxelColor = texelFetch (volumeTexture, ivec3 (x, dstPos.y, z), SrcMipLevel);

		//		if (voxelColor.a == 0) {
		//			continue;
		//		}

		//		resultColor += voxelColor;
		//		contributionCount ++;
		//	}
		//}

		//imageStore (dstImageMip, dstPos, vec4(resultColor.xyz / contributionCount, 0.0));
	}
}