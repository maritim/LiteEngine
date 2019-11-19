#ifndef RSMDIRECTIONALLIGHTACCUMULATIONRENDERPASS_H
#define RSMDIRECTIONALLIGHTACCUMULATIONRENDERPASS_H

#include "RSMAccumulationRenderPass.h"

#include <string>

class RSMDirectionalLightAccumulationRenderPass : public RSMAccumulationRenderPass
{
	DECLARE_RENDER_PASS(RSMDirectionalLightAccumulationRenderPass)

protected:
	std::string _staticShaderName;
	std::string _animationShaderName;

public:
	RSMDirectionalLightAccumulationRenderPass ();

	void Init (const RenderSettings& settings);
protected:
	void LockShader (int sceneLayers);
	Camera* GetLightCamera (const RenderScene* renderScene, const RenderLightObject* renderLightObject);

	std::vector<PipelineAttribute> GetCustomAttributes (const RenderSettings& settings) const;
};

#endif