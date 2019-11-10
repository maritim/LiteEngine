#ifndef DEFERREDSPOTLIGHTSHADOWMAPRENDERPASS_H
#define DEFERREDSPOTLIGHTSHADOWMAPRENDERPASS_H

#include "RenderPasses/ShadowMap/SpotLightShadowMapRenderPass.h"

#include <string>

class ENGINE_API DeferredSpotLightShadowMapRenderPass : public SpotLightShadowMapRenderPass
{
	DECLARE_RENDER_PASS(DeferredSpotLightShadowMapRenderPass)

protected:
	std::string _staticShaderName;
	std::string _animationShaderName;

public:
	DeferredSpotLightShadowMapRenderPass ();

	void Init (const RenderSettings& settings);
protected:
	void LockShader (int sceneLayers);
};

#endif