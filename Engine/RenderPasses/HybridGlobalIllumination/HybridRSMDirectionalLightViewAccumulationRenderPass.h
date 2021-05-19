#ifndef HYBRIDRSMDIRECTIONALLIGHTVIEWACCUMULATIONRENDERPASS_H
#define HYBRIDRSMDIRECTIONALLIGHTVIEWACCUMULATIONRENDERPASS_H

#include "RenderPasses/ReflectiveShadowMapping/RSMDirectionalLightViewAccumulationRenderPass.h"

#include "Utils/Sequences/HaltonGenerator.h"

class HybridRSMDirectionalLightViewAccumulationRenderPass : public RSMDirectionalLightViewAccumulationRenderPass
{
	DECLARE_RENDER_PASS(HybridRSMDirectionalLightViewAccumulationRenderPass)

protected:
	HaltonGenerator _haltonGenerator;

public:
	HybridRSMDirectionalLightViewAccumulationRenderPass ();

protected:
	void ShadowMapGeometryPass (const RenderScene* renderScene, const Camera* lightCamera,
		const RenderSettings& settings, const RenderLightObject* renderLightObject);

	void UpdateCamera (const Camera* lightCamera, const RenderSettings& settings);
};

#endif