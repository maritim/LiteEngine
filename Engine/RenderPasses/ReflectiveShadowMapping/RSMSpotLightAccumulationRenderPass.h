#ifndef RSMSPOTLIGHTACCUMULATIONRENDERPASS_H
#define RSMSPOTLIGHTACCUMULATIONRENDERPASS_H

#include "RSMAccumulationRenderPass.h"

#include "Core/Resources/Resource.h"
#include "Renderer/RenderViews/ShaderView.h"

class RSMSpotLightAccumulationRenderPass : public RSMAccumulationRenderPass
{
	DECLARE_RENDER_PASS(RSMSpotLightAccumulationRenderPass)

protected:
	Resource<ShaderView> _staticShaderView;
	Resource<ShaderView> _animationShaderView;

public:
	void Init (const RenderSettings& settings);
protected:
	void LockShader (int sceneLayers);
	Camera* GetLightCamera (const RenderScene* renderScene, const RenderLightObject* renderLightObject);

	std::vector<PipelineAttribute> GetCustomAttributes (const RenderLightObject* renderLightObject) const;
};

#endif