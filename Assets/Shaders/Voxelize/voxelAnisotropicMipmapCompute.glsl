#version 430
layout (local_size_x = 4, local_size_y = 4, local_size_z = 4) in;

// Compute-based mipmapping inspired by 

/*  Workgroup Size Stats
1^3 167.00 ms
2^3  21.23 ms
4^3   3.77 ms
8^3   4.02 ms  */

uniform int DstMipRes;

uniform sampler3D srcTextureMip;

layout(binding = 0, rgba8) uniform writeonly image3D dstImageMip;

vec4[8] fetchTexels(ivec3 pos) {
  return vec4[8] (texelFetch(srcTextureMip, pos + ivec3(1, 1, 1), 0),  // 0
                  texelFetch(srcTextureMip, pos + ivec3(1, 1, 0), 0),  // 1
                  texelFetch(srcTextureMip, pos + ivec3(1, 0, 1), 0),  // 2
                  texelFetch(srcTextureMip, pos + ivec3(1, 0, 0), 0),  // 3
                  texelFetch(srcTextureMip, pos + ivec3(0, 1, 1), 0),  // 4
                  texelFetch(srcTextureMip, pos + ivec3(0, 1, 0), 0),  // 5
                  texelFetch(srcTextureMip, pos + ivec3(0, 0, 1), 0),  // 6
                  texelFetch(srcTextureMip, pos + ivec3(0, 0, 0), 0)); // 7
}

void main() 
{
	if (gl_GlobalInvocationID.x < DstMipRes
		&& gl_GlobalInvocationID.y < DstMipRes
		&& gl_GlobalInvocationID.z < DstMipRes) {

		ivec3 dstPos = ivec3(gl_GlobalInvocationID);
		ivec3 srcPos = dstPos * 2;
  
		vec4 values[8] = fetchTexels(srcPos);

		const ivec4[6] faces = ivec4[6] (
			ivec4 (4, 5, 6, 7), ivec4 (0, 1, 2, 3),
			ivec4 (2, 3, 6, 7), ivec4 (0, 1, 4, 5),
			ivec4 (1, 3, 5, 7), ivec4 (0, 2, 4, 6)
		);

		const ivec4[6] backFaces = ivec4[6] (
			ivec4 (0, 1, 2, 3), ivec4 (4, 5, 6, 7),
			ivec4 (0, 1, 4, 5), ivec4 (2, 3, 6, 7),
			ivec4 (0, 2, 4, 6), ivec4 (1, 3, 5, 7)
		);

		for (int faceIndex = 0; faceIndex < 6; faceIndex ++) {

			vec4 finalColor = vec4 (0);
			float contributionCount = 0;

			for (int i=0; i<4; i++) {

				finalColor +=  values [faces [faceIndex][i]] +
					values [backFaces [faceIndex][i]] * (1.0 - values [faces [faceIndex][i]].a);
			}

			ivec3 voxelPos = ivec3 (dstPos.x + DstMipRes * faceIndex, dstPos.y, dstPos.z);
			imageStore(dstImageMip, voxelPos, finalColor / 4.0);// vec4(finalColor.rgb / contributionCount, finalColor.a / 4.0));
		}
	}
}