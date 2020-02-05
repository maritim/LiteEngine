#ifndef DEFERREDSPOTLIGHTRENDERPASS_H
#define DEFERREDSPOTLIGHTRENDERPASS_H

#include "RenderPasses/VolumetricLightRenderPass.h"

#include "Core/Resources/Resource.h"
#include "Renderer/RenderViews/ShaderView.h"

class ENGINE_API DeferredSpotLightRenderPass : public VolumetricLightRenderPass
{
	DECLARE_RENDER_PASS(DeferredSpotLightRenderPass)

protected:
	Resource<ShaderView> _shaderView;
	Resource<ShaderView> _shadowShaderView;

public:
	void Init (const RenderSettings& settings);

	void Clear ();
protected:
	void LockShader (const RenderLightObject*);

	std::vector<PipelineAttribute> GetCustomAttributes (const RenderLightObject*) const;
};

#endif