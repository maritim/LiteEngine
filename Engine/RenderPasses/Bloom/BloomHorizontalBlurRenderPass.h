#ifndef BLOOMHORIZONTALBLURRENDERPASS_H
#define BLOOMHORIZONTALBLURRENDERPASS_H

#include "RenderPasses/Blur/HorizontalGaussianBlurRenderPass.h"

class ENGINE_API BloomHorizontalBlurRenderPass : public HorizontalGaussianBlurRenderPass
{
	DECLARE_RENDER_PASS(BloomHorizontalBlurRenderPass)

public:
	bool IsAvailable (const RenderScene* renderScene, const Camera* camera,
		const RenderSettings& settings, const RenderVolumeCollection* rvc) const;
protected:
	glm::ivec2 GetPostProcessVolumeResolution (const RenderSettings& settings) const;
};

#endif