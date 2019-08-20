#ifndef BLOOMVERTICALBLURCONTAINERRENDERSUBPASS_H
#define BLOOMVERTICALBLURCONTAINERRENDERSUBPASS_H

#include "RenderPasses/Blur/VerticalGaussianBlurContainerRenderSubPass.h"

class ENGINE_API BloomVerticalBlurContainerRenderSubPass : public VerticalGaussianBlurContainerRenderSubPass
{
public:
	bool IsAvailable (const RenderScene* renderScene, const Camera* camera,
		const RenderSettings& settings, const RenderVolumeCollection* rvc) const;
protected:
	glm::ivec2 GetPostProcessVolumeResolution (const RenderSettings& settings) const;
};

#endif