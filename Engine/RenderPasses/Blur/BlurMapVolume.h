#ifndef BLURMAPVOLUME_H
#define BLURMAPVOLUME_H

#include "RenderPasses/PostProcess/PostProcessMapVolume.h"

class BlurMapVolume : public PostProcessMapVolume
{
public:
	bool Init (std::size_t bufferWidth, std::size_t bufferHeight);

	void BindForReading ();

	std::vector<PipelineAttribute> GetCustomAttributes () const;
};

#endif