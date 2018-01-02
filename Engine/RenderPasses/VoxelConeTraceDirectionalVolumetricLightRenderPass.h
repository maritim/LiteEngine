#ifndef VOXELCONETRACEDIRECTIONALVOLUMETRICLIGHTRENDERPASS_H
#define VOXELCONETRACEDIRECTIONALVOLUMETRICLIGHTRENDERPASS_H

#include "DirectionalVolumetricLightRenderPass.h"

#include <string>

class VoxelConeTraceDirectionalVolumetricLightRenderPass : public DirectionalVolumetricLightRenderPass
{
protected:
	std::string _shadowShaderName;

public:
	VoxelConeTraceDirectionalVolumetricLightRenderPass ();

	void Init ();
protected:
	void LockShader (const VolumetricLight*);
};

#endif