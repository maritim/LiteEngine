#ifndef INDIRECTDIFFUSELIGHTMAPVOLUME_H
#define INDIRECTDIFFUSELIGHTMAPVOLUME_H

#include "RenderPasses/PostProcess/PostProcessMapVolume.h"

class IndirectDiffuseLightMapVolume : public PostProcessMapVolume
{
public:
	std::vector<PipelineAttribute> GetCustomAttributes () const;
};

#endif