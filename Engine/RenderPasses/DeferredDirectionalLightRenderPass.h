#ifndef DEFERREDDIRECTIONALLIGHTRENDERPASS_H
#define DEFERREDDIRECTIONALLIGHTRENDERPASS_H

#include "RenderPasses/DirectionalVolumetricLightRenderPass.h"

#include <string>

class ENGINE_API DeferredDirectionalLightRenderPass : public DirectionalVolumetricLightRenderPass
{
	DECLARE_RENDER_PASS(DeferredDirectionalLightRenderPass)

protected:
	std::string _shaderName;
	std::string _shadowShaderName;

public:
	DeferredDirectionalLightRenderPass ();

	void Init (const RenderSettings& settings);

	void Clear ();
protected:
	void LockShader (const RenderLightObject*);

	std::vector<PipelineAttribute> GetCustomAttributes (const RenderSettings& settings) const;
};

#endif