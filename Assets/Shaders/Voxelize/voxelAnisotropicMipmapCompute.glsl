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
				// float contribution = values [faces[faceIndex][i]].a == 0 ? 0.0 : 1.0;

				finalColor +=  values [faces [faceIndex][i]] +
					values [backFaces [faceIndex][i]] * (1.0 - values [faces [faceIndex][i]].a);

				// finalColor.rgb += voxelColor.rgb * contribution;
				// contributionCount += contribution; 
				// finalColor.a += voxelColor.a;
			}

			ivec3 voxelPos = ivec3 (dstPos.x + DstMipRes * faceIndex, dstPos.y, dstPos.z);
			imageStore(dstImageMip, voxelPos, finalColor / 4.0);// vec4(finalColor.rgb / contributionCount, finalColor.a / 4.0));
		}

		// imageStore(dstImageMip, dstPos, (
		// 	values [0] + values [4] * (1.0 - values [0].a),
		// 	values [1] + values [5] * (1.0 - values [1].a),
		// 	values [2] + values [6] * (1.0 - values [2].a),
		// 	values [3] + values [7] * (1.0 - values [3].a)
		// ) / 4.0);

		// imageStore(dstImageMip, dstPos + ivec3 (DstMipRes, 0, 0), (
		// 	values [4] + values [0] * (1.0 - values [4].a),
		// 	values [5] + values [1] * (1.0 - values [5].a),
		// 	values [6] + values [2] * (1.0 - values [6].a),
		// 	values [7] + values [3] * (1.0 - values [7].a)
		// ) / 4.0);

		// imageStore(dstImageMip, dstPos + ivec3 (DstMipRes * 2, 0, 0), (
		// 	values [0] + values [2] * (1.0 - values [0].a),
		// 	values [1] + values [3] * (1.0 - values [1].a),
		// 	values [4] + values [6] * (1.0 - values [4].a),
		// 	values [5] + values [7] * (1.0 - values [5].a)
		// ) / 4.0);

		// imageStore(dstImageMip, dstPos + ivec3 (DstMipRes * 3, 0, 0), (
		// 	values [2] + values [0] * (1.0 - values [2].a),
		// 	values [3] + values [1] * (1.0 - values [3].a),
		// 	values [6] + values [4] * (1.0 - values [6].a),
		// 	values [7] + values [5] * (1.0 - values [7].a)
		// ) / 4.0);

		// imageStore(dstImageMip, dstPos + ivec3 (DstMipRes * 4, 0, 0), (
		// 	values [0] + values [1] * (1.0 - values [0].a),
		// 	values [2] + values [3] * (1.0 - values [2].a),
		// 	values [4] + values [5] * (1.0 - values [4].a),
		// 	values [6] + values [7] * (1.0 - values [6].a)
		// ) / 4.0);

		// imageStore(dstImageMip, dstPos + ivec3 (DstMipRes * 5, 0, 0), (
		// 	values [1] + values [0] * (1.0 - values [1].a),
		// 	values [3] + values [2] * (1.0 - values [3].a),
		// 	values [5] + values [4] * (1.0 - values [5].a),
		// 	values [7] + values [6] * (1.0 - values [7].a)
		// ) / 4.0);
	}
}