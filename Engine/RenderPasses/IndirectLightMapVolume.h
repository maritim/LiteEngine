#ifndef INDIRECTLIGHTMAPVOLUME_H
#define INDIRECTLIGHTMAPVOLUME_H

#include "RenderPasses/PostProcess/PostProcessMapVolume.h"

class IndirectLightMapVolume : public PostProcessMapVolume
{
public:
	std::vector<PipelineAttribute> GetCustomAttributes () const;
};

#endif