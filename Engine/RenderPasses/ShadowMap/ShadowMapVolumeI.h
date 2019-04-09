#ifndef SHADOWMAPVOLUMEI_H
#define SHADOWMAPVOLUMEI_H

#include "Renderer/RenderVolumeI.h"

class ShadowMapVolumeI : public RenderVolumeI
{
public:
	virtual void BindForShadowMapCatch (std::size_t cascadedLevel = 0) = 0;
};

#endif