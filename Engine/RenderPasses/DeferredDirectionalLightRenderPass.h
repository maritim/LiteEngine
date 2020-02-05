#ifndef DEFERREDDIRECTIONALLIGHTRENDERPASS_H
#define DEFERREDDIRECTIONALLIGHTRENDERPASS_H

#include "RenderPasses/DirectionalVolumetricLightRenderPass.h"

#include "Core/Resources/Resource.h"
#include "Renderer/RenderViews/ShaderView.h"

class ENGINE_API DeferredDirectionalLightRenderPass : public DirectionalVolumetricLightRenderPass
{
	DECLARE_RENDER_PASS(DeferredDirectionalLightRenderPass)

protected:
	Resource<ShaderView> _shaderView;
	Resource<ShaderView> _shadowShaderView;

public:
	void Init (const RenderSettings& settings);

	void Clear ();
protected:
	void LockShader (const RenderLightObject*);

	std::vector<PipelineAttribute> GetCustomAttributes (const RenderSettings& settings) const;
};

#endif