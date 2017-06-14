#version 430
layout (local_size_x = 4, local_size_y = 4, local_size_z = 4) in;

// Compute-based mipmapping inspired by 

/*  Workgroup Size Stats
1^3 167.00 ms
2^3  21.23 ms
4^3   3.77 ms
8^3   4.02 ms  */

uniform int SrcMipLevel;
uniform int DstMipRes;

uniform sampler3D volumeTexture;

layout(binding = 0, rgba8) uniform writeonly image3D dstImageMip;

vec4[8] fetchTexels(int dir, ivec3 pos) {
  return vec4[8] (texelFetch(volumeTexture, pos + ivec3(1, 1, 1), SrcMipLevel),  // 0
                  texelFetch(volumeTexture, pos + ivec3(1, 1, 0), SrcMipLevel),  // 1
                  texelFetch(volumeTexture, pos + ivec3(1, 0, 1), SrcMipLevel),  // 2
                  texelFetch(volumeTexture, pos + ivec3(1, 0, 0), SrcMipLevel),  // 3
                  texelFetch(volumeTexture, pos + ivec3(0, 1, 1), SrcMipLevel),  // 4
                  texelFetch(volumeTexture, pos + ivec3(0, 1, 0), SrcMipLevel),  // 5
                  texelFetch(volumeTexture, pos + ivec3(0, 0, 1), SrcMipLevel),  // 6
                  texelFetch(volumeTexture, pos + ivec3(0, 0, 0), SrcMipLevel)); // 7
}

void main() 
{
	if (gl_GlobalInvocationID.x < DstMipRes
		&& gl_GlobalInvocationID.y < DstMipRes
		&& gl_GlobalInvocationID.z < DstMipRes) {

		ivec3 dstPos = ivec3(gl_GlobalInvocationID);
		ivec3 srcPos = dstPos * 2;
  
		vec4 values[8] = fetchTexels(0, srcPos);

		vec3 finalColor = vec3 (0);
		float contributionCount = 0;

		for (int i=0; i<8; i++) {
			vec3 contribution = values [i].a == 0 ? vec3 (0) : vec3 (1);

			finalColor += values [i].rgb * contribution;
			contributionCount += contribution.x; 
		}

		float alpha = contributionCount > 0 ? 1.0 : 0.0;

		imageStore(dstImageMip, dstPos, vec4(finalColor / contributionCount, alpha));
	}
}