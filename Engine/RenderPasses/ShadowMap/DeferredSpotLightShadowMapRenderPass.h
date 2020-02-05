#ifndef DEFERREDSPOTLIGHTSHADOWMAPRENDERPASS_H
#define DEFERREDSPOTLIGHTSHADOWMAPRENDERPASS_H

#include "RenderPasses/ShadowMap/SpotLightShadowMapRenderPass.h"

#include "Core/Resources/Resource.h"
#include "Renderer/RenderViews/ShaderView.h"

class ENGINE_API DeferredSpotLightShadowMapRenderPass : public SpotLightShadowMapRenderPass
{
	DECLARE_RENDER_PASS(DeferredSpotLightShadowMapRenderPass)

protected:
	Resource<ShaderView> _staticShaderView;
	Resource<ShaderView> _animationShaderView;

public:
	void Init (const RenderSettings& settings);
protected:
	void LockShader (int sceneLayers);
};

#endif