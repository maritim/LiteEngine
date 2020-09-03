#ifndef RSMINTERPOLATEDINDIRECTDIFFUSELIGHTMAPVOLUME_H
#define RSMINTERPOLATEDINDIRECTDIFFUSELIGHTMAPVOLUME_H

#include "RenderPasses/IndirectDiffuseLightMapVolume.h"

class RSMInterpolatedIndirectDiffuseLightMapVolume : public IndirectDiffuseLightMapVolume
{
public:
	bool Init (const glm::ivec2& size);	
};

#endif