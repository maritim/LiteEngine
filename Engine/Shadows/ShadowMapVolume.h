#ifndef SHADOWMAPVOLUME_H
#define SHADOWMAPVOLUME_H

#include "Renderer/RenderVolumeI.h"

class ShadowMapVolume : public RenderVolumeI
{
public:
	ShadowMapVolume ();	
	virtual ~ShadowMapVolume ();

	virtual bool Init (std::size_t cascadedLevelsCount) = 0;
	virtual void BindForShadowMapCatch (std::size_t cascadedLevel = 0) = 0;
	virtual void EndDrawing () = 0;

	virtual void BindForReading () = 0;
	virtual std::vector<PipelineAttribute> GetCustomAttributes () = 0;
};

#endif