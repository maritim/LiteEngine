#version 430
layout (local_size_x = 24, local_size_y = 4, local_size_z = 4) in;

// Compute-based mipmapping inspired by 

/*  Workgroup Size Stats
1^3 167.00 ms
2^3  21.23 ms
4^3   3.77 ms
8^3   4.02 ms  */

uniform int DstMipRes;

uniform int srcMipLevel;
uniform sampler3D srcTextureMip;

layout(binding = 0, rgba8) uniform writeonly image3D dstImageMip;

vec4[8] fetchTexels(ivec3 pos) {
  return vec4[8] (texelFetch(srcTextureMip, pos + ivec3(1, 1, 1), srcMipLevel),  // 0
                  texelFetch(srcTextureMip, pos + ivec3(1, 1, 0), srcMipLevel),  // 1
                  texelFetch(srcTextureMip, pos + ivec3(1, 0, 1), srcMipLevel),  // 2
                  texelFetch(srcTextureMip, pos + ivec3(1, 0, 0), srcMipLevel),  // 3
                  texelFetch(srcTextureMip, pos + ivec3(0, 1, 1), srcMipLevel),  // 4
                  texelFetch(srcTextureMip, pos + ivec3(0, 1, 0), srcMipLevel),  // 5
                  texelFetch(srcTextureMip, pos + ivec3(0, 0, 1), srcMipLevel),  // 6
                  texelFetch(srcTextureMip, pos + ivec3(0, 0, 0), srcMipLevel)); // 7
}

void main() 
{
	if (gl_GlobalInvocationID.x < DstMipRes * 6
		&& gl_GlobalInvocationID.y < DstMipRes
		&& gl_GlobalInvocationID.z < DstMipRes) {

		ivec3 dstPos = ivec3(gl_GlobalInvocationID);
		ivec3 srcPos = dstPos * 2;
  
		vec4 values[8] = fetchTexels(srcPos);

		vec4 finalColor = vec4 (0);
		float contributionCount = 0;

		for (int i=0; i<8; i++) {
			float contribution = values [i].a == 0 ? 0.0 : 1.0;

			finalColor.rgb += values [i].rgb * contribution;
			contributionCount += contribution;
			finalColor.a += values [i].a;
		}

		imageStore(dstImageMip, dstPos, vec4(finalColor.rgb / contributionCount, finalColor.a / 8.0));
	}
}