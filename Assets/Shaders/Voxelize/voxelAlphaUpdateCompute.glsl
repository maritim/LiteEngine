#version 430
layout (local_size_x = 4, local_size_y = 4, local_size_z = 4) in;

layout(binding = 0, rgba8) uniform image3D voxelVolume;

void main() 
{
	ivec3 size = imageSize (voxelVolume);

	if (gl_GlobalInvocationID.x < size.x
		&& gl_GlobalInvocationID.y < size.x
		&& gl_GlobalInvocationID.z < size.x) {

		ivec3 voxelPos = ivec3(gl_GlobalInvocationID);
  
		vec4 voxelColor = imageLoad (voxelVolume, voxelPos);

		if (voxelColor.a == 0.0) {
			return;
		}

		voxelColor.a = 1.0;

		imageStore (voxelVolume, voxelPos, voxelColor);
	}
}