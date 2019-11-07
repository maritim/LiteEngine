#ifndef DEFERREDSPOTLIGHTRENDERPASS_H
#define DEFERREDSPOTLIGHTRENDERPASS_H

#include "RenderPasses/VolumetricLightRenderPass.h"

#include <string>

class ENGINE_API DeferredSpotLightRenderPass : public VolumetricLightRenderPass
{
	DECLARE_RENDER_PASS(DeferredSpotLightRenderPass)

protected:
	std::string _shaderName;
	std::string _shadowShaderName;

public:
	DeferredSpotLightRenderPass ();

	void Init (const RenderSettings& settings);

	void Clear ();
protected:
	void LockShader (const RenderLightObject*);
};

#endif