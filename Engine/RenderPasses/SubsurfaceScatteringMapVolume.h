#ifndef SUBSURFACESCATTERINGMAPVOLUME_H
#define SUBSURFACESCATTERINGMAPVOLUME_H

#include "RenderPasses/PostProcess/PostProcessMapVolume.h"

class SubsurfaceScatteringMapVolume : public PostProcessMapVolume
{
public:
	std::vector<PipelineAttribute> GetCustomAttributes () const;
};

#endif