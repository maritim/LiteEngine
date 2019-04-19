#ifndef EXPONENTIALSHADOWMAPBLURVOLUME_H
#define EXPONENTIALSHADOWMAPBLURVOLUME_H

#include "RenderPasses/PostProcess/PostProcessMapVolume.h"

class ExponentialShadowMapBlurVolume : public PostProcessMapVolume
{
protected:
	glm::ivec2 _resolution;

public:
	bool Init (std::size_t bufferWidth, std::size_t bufferHeight);

	std::vector<PipelineAttribute> GetCustomAttributes () const;
};

#endif