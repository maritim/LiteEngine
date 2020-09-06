#ifndef EXPONENTIALSHADOWMAPBLURVOLUME_H
#define EXPONENTIALSHADOWMAPBLURVOLUME_H

#include "RenderPasses/FramebufferRenderVolume.h"

class ExponentialShadowMapBlurVolume : public FramebufferRenderVolume
{
protected:
	glm::ivec2 _resolution;

public:
	bool Init (std::size_t bufferWidth, std::size_t bufferHeight);

	const std::vector<PipelineAttribute>& GetCustomAttributes () const;
};

#endif