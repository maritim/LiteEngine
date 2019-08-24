#ifndef BLOOMVERTICALBLURRENDERPASS_H
#define BLOOMVERTICALBLURRENDERPASS_H

#include "RenderPasses/Blur/VerticalGaussianBlurRenderPass.h"

class ENGINE_API BloomVerticalBlurRenderPass : public VerticalGaussianBlurRenderPass
{
	DECLARE_RENDER_PASS(BloomVerticalBlurRenderPass)

public:
	bool IsAvailable (const RenderScene* renderScene, const Camera* camera,
		const RenderSettings& settings, const RenderVolumeCollection* rvc) const;
protected:
	glm::ivec2 GetPostProcessVolumeResolution (const RenderSettings& settings) const;
};

#endif