#ifndef SSAOMAPVOLUME_H
#define SSAOMAPVOLUME_H

#include "RenderPasses/PostProcess/PostProcessMapVolume.h"

class ENGINE_API SSAOMapVolume : public PostProcessMapVolume
{
public:
	bool Init (std::size_t bufferWidth, std::size_t bufferHeight);

	void BindForReading ();

	std::vector<PipelineAttribute> GetCustomAttributes () const;
};

#endif