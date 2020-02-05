#ifndef RSMDIRECTIONALLIGHTACCUMULATIONRENDERPASS_H
#define RSMDIRECTIONALLIGHTACCUMULATIONRENDERPASS_H

#include "RSMAccumulationRenderPass.h"

#include "Core/Resources/Resource.h"
#include "Renderer/RenderViews/ShaderView.h"

class RSMDirectionalLightAccumulationRenderPass : public RSMAccumulationRenderPass
{
	DECLARE_RENDER_PASS(RSMDirectionalLightAccumulationRenderPass)

protected:
	Resource<ShaderView> _staticShaderView;
	Resource<ShaderView> _animationShaderView;

public:
	void Init (const RenderSettings& settings);
protected:
	void LockShader (int sceneLayers);
	Camera* GetLightCamera (const RenderScene* renderScene, const RenderLightObject* renderLightObject);

	std::vector<PipelineAttribute> GetCustomAttributes (const RenderSettings& settings) const;
};

#endif