#ifndef VCTDIRECTIONALLIGHTRENDERPASS_H
#define VCTDIRECTIONALLIGHTRENDERPASS_H

#include "RenderPasses/DirectionalVolumetricLightContainerRenderSubPass.h"

#include <string>

class VCTDirectionalLightRenderPass : public DirectionalVolumetricLightContainerRenderSubPass
{
protected:
	std::string _shadowShaderName;

public:
	VCTDirectionalLightRenderPass ();

	void Init (const RenderSettings& settings);

	void Clear ();
protected:
	void LockShader (const RenderLightObject*);

	std::vector<PipelineAttribute> GetCustomAttributes (const RenderSettings& settings) const;
};

#endif