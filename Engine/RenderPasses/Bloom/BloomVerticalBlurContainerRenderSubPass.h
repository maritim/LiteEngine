#ifndef BLOOMVERTICALBLURCONTAINERRENDERSUBPASS_H
#define BLOOMVERTICALBLURCONTAINERRENDERSUBPASS_H

#include "RenderPasses/Blur/VerticalGaussianBlurContainerRenderSubPass.h"

class BloomVerticalBlurContainerRenderSubPass : public VerticalGaussianBlurContainerRenderSubPass
{
public:
	bool IsAvailable (const Scene* scene, const Camera* camera,
		const RenderSettings& settings, const RenderVolumeCollection* rvc) const;
protected:
	glm::ivec2 GetPostProcessVolumeResolution (const RenderSettings& settings) const;
};

#endif