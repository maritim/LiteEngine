#version 430
layout (local_size_x = 4, local_size_y = 4, local_size_z = 4) in;

layout(binding = 0, rgba32f) uniform writeonly image3D lpvVolumeR;
layout(binding = 1, rgba32f) uniform writeonly image3D lpvVolumeG;
layout(binding = 2, rgba32f) uniform writeonly image3D lpvVolumeB;

uniform isampler3D volumeTextureR;
uniform isampler3D volumeTextureG;
uniform isampler3D volumeTextureB;
uniform ivec3 volumeSize;

#define SH_I2F 0.001

#define imgLoad(img, pos) \
	vec4 (texelFetch(img, ivec3(pos.x * 4, pos.y, pos.z), 0).x * SH_I2F, \
		texelFetch(img, ivec3(pos.x * 4 + 1, pos.y, pos.z), 0).x * SH_I2F, \
		texelFetch(img, ivec3(pos.x * 4 + 2, pos.y, pos.z), 0).x * SH_I2F, \
		texelFetch(img, ivec3(pos.x * 4 + 3, pos.y, pos.z), 0).x * SH_I2F)

void main() 
{
	if (gl_GlobalInvocationID.x < volumeSize.x
		&& gl_GlobalInvocationID.y < volumeSize.y
		&& gl_GlobalInvocationID.z < volumeSize.z) {

		ivec3 volumePos = ivec3(gl_GlobalInvocationID);

		vec4 resultR = imgLoad (volumeTextureR, volumePos);
		vec4 resultG = imgLoad (volumeTextureG, volumePos);
		vec4 resultB = imgLoad (volumeTextureB, volumePos);

		imageStore(lpvVolumeR, volumePos, resultR);
		imageStore(lpvVolumeG, volumePos, resultG);
		imageStore(lpvVolumeB, volumePos, resultB);
	}
}