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

	void Clear ();
protected:
	void LockShader (const VolumetricLight*);

	std::vector<PipelineAttribute> GetCustomAttributes ();
};

#endif