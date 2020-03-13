#ifndef INDIRECTSPECULARLIGHTMAPVOLUME_H
#define INDIRECTSPECULARLIGHTMAPVOLUME_H

#include "RenderPasses/PostProcess/PostProcessMapVolume.h"

class IndirectSpecularLightMapVolume : public PostProcessMapVolume
{
public:
	std::vector<PipelineAttribute> GetCustomAttributes () const;
};

#endif