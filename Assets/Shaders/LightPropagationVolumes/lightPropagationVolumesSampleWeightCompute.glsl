#version 430
layout (local_size_x = 4, local_size_y = 4, local_size_z = 4) in;

uniform layout (binding = 0, r32i) coherent volatile iimage3D lpvVolumeR;
uniform layout (binding = 1, r32i) coherent volatile iimage3D lpvVolumeG;
uniform layout (binding = 2, r32i) coherent volatile iimage3D lpvVolumeB;

uniform isampler3D volumeTextureR;
uniform isampler3D volumeTextureG;
uniform isampler3D volumeTextureB;
uniform usampler3D sampleCountTexture;
uniform ivec3 volumeSize;

#define SH_F2I 1000.0
#define SH_I2F 0.001

#define imgLoad(img, pos) \
	vec4 (texelFetch(img, ivec3(pos.x * 4, pos.y, pos.z), 0).x * SH_I2F, \
		texelFetch(img, ivec3(pos.x * 4 + 1, pos.y, pos.z), 0).x * SH_I2F, \
		texelFetch(img, ivec3(pos.x * 4 + 2, pos.y, pos.z), 0).x * SH_I2F, \
		texelFetch(img, ivec3(pos.x * 4 + 3, pos.y, pos.z), 0).x * SH_I2F)

#define imgStore(img, pos, data) \
	imageAtomicExchange(img, ivec3(pos.x * 4, pos.y, pos.z), int(data.x * SH_F2I)); \
	imageAtomicExchange(img, ivec3(pos.x * 4 + 1, pos.y, pos.z), int(data.y * SH_F2I)); \
	imageAtomicExchange(img, ivec3(pos.x * 4 + 2, pos.y, pos.z), int(data.z * SH_F2I)); \
	imageAtomicExchange(img, ivec3(pos.x * 4 + 3, pos.y, pos.z), int(data.w * SH_F2I));

void main()
{
	if (gl_GlobalInvocationID.x < volumeSize.x
		&& gl_GlobalInvocationID.y < volumeSize.y
		&& gl_GlobalInvocationID.z < volumeSize.z) {

		ivec3 volumePos = ivec3(gl_GlobalInvocationID);

		vec4 resultR = imgLoad (volumeTextureR, volumePos);
		vec4 resultG = imgLoad (volumeTextureG, volumePos);
		vec4 resultB = imgLoad (volumeTextureB, volumePos);

		float sampleCount = float (texelFetch (sampleCountTexture, volumePos, 0).x);

		if (sampleCount == 0) {
			return;
		}

		resultR /= sampleCount;
		resultG /= sampleCount;
		resultB /= sampleCount;

		imgStore(lpvVolumeR, volumePos, resultR);
		imgStore(lpvVolumeG, volumePos, resultG);
		imgStore(lpvVolumeB, volumePos, resultB);
	}
}