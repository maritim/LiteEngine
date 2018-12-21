#ifndef SSAOMAPVOLUME_H
#define SSAOMAPVOLUME_H

#include "RenderPasses/PostProcess/PostProcessMapVolume.h"

class SSAOMapVolume : public PostProcessMapVolume
{
public:
	bool Init (std::size_t bufferWidth, std::size_t bufferHeight);

	void BindForReading ();

	std::vector<PipelineAttribute> GetCustomAttributes () const;
};

#endif