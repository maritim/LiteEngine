#ifndef RSMSPOTLIGHTACCUMULATIONRENDERPASS_H
#define RSMSPOTLIGHTACCUMULATIONRENDERPASS_H

#include "RSMAccumulationRenderPass.h"

#include <string>

class RSMSpotLightAccumulationRenderPass : public RSMAccumulationRenderPass
{
	DECLARE_RENDER_PASS(RSMSpotLightAccumulationRenderPass)

protected:
	std::string _staticShaderName;
	std::string _animationShaderName;

public:
	RSMSpotLightAccumulationRenderPass ();

	void Init (const RenderSettings& settings);
protected:
	void LockShader (int sceneLayers);
	Camera* GetLightCamera (const RenderScene* renderScene, const RenderLightObject* renderLightObject);

	std::vector<PipelineAttribute> GetCustomAttributes (const RenderLightObject* renderLightObject) const;
};

#endif