#ifndef VOXELCONETRACEDIRECTIONALLIGHTRENDERPASS_H
#define VOXELCONETRACEDIRECTIONALLIGHTRENDERPASS_H

#include "RenderPasses/DirectionalVolumetricLightContainerRenderSubPass.h"

#include <string>

class VoxelConeTraceDirectionalLightRenderPass : public DirectionalVolumetricLightContainerRenderSubPass
{
protected:
	std::string _shadowShaderName;

public:
	VoxelConeTraceDirectionalLightRenderPass ();

	void Init ();
protected:
	void LockShader (const VolumetricLight*);
};

#endif