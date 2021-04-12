#ifndef RSMDIRECTIONALLIGHTVIEWACCUMULATIONRENDERPASS_H
#define RSMDIRECTIONALLIGHTVIEWACCUMULATIONRENDERPASS_H

#include "RSMDirectionalLightAccumulationRenderPass.h"

#include "Core/Resources/Resource.h"
#include "Renderer/RenderViews/ShaderView.h"

class RSMDirectionalLightViewAccumulationRenderPass : public RSMDirectionalLightAccumulationRenderPass
{
	DECLARE_RENDER_PASS(RSMDirectionalLightViewAccumulationRenderPass)

public:
	void Init (const RenderSettings& settings);
protected:
	void StartShadowMapPass (const Camera* lightCamera);
};

#endif