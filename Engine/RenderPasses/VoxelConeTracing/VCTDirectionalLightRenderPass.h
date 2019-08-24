#ifndef VCTDIRECTIONALLIGHTRENDERPASS_H
#define VCTDIRECTIONALLIGHTRENDERPASS_H

#include "RenderPasses/DirectionalVolumetricLightRenderPass.h"

#include <string>

class VCTDirectionalLightRenderPass : public DirectionalVolumetricLightRenderPass
{
	DECLARE_RENDER_PASS(VCTDirectionalLightRenderPass)

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