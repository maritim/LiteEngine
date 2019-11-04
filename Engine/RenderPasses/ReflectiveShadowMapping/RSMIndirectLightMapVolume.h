#ifndef RSMINDIRECTLIGHTMAPVOLUME_H
#define RSMINDIRECTLIGHTMAPVOLUME_H

#include "RenderPasses/PostProcess/PostProcessMapVolume.h"

class RSMIndirectLightMapVolume : public PostProcessMapVolume
{
public:
	std::vector<PipelineAttribute> GetCustomAttributes () const;
};

#endif