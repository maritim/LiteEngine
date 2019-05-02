#ifndef BLOOMHORIZONTALBLURCONTAINERRENDERSUBPASS_H
#define BLOOMHORIZONTALBLURCONTAINERRENDERSUBPASS_H

#include "RenderPasses/Blur/HorizontalGaussianBlurContainerRenderSubPass.h"

class BloomHorizontalBlurContainerRenderSubPass : public HorizontalGaussianBlurContainerRenderSubPass
{
public:
	bool IsAvailable (const Scene* scene, const Camera* camera,
		const RenderSettings& settings, const RenderVolumeCollection* rvc) const;
protected:
	glm::ivec2 GetPostProcessVolumeResolution (const RenderSettings& settings) const;
};

#endif