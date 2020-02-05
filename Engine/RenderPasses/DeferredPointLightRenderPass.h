#ifndef DEFERREDPOINTLIGHTRENDERPASS_H
#define DEFERREDPOINTLIGHTRENDERPASS_H

#include "RenderPasses/VolumetricLightRenderPass.h"

#include "Core/Resources/Resource.h"
#include "Renderer/RenderViews/ShaderView.h"

class ENGINE_API DeferredPointLightRenderPass : public VolumetricLightRenderPass
{
	DECLARE_RENDER_PASS(DeferredPointLightRenderPass)

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