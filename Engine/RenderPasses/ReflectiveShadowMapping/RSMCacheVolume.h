#ifndef RSMCACHEVOLUME_H
#define RSMCACHEVOLUME_H

#include "RenderPasses/FrameBuffer2DVolume.h"

#include "Systems/Camera/Camera.h"

#define REFLECTIVE_SHADOW_MAPPING_CACHE_VOLUME_NOT_INIT 370

class RSMCacheVolume : public FrameBuffer2DVolume
{
public:
	bool Init (const glm::ivec2& size);

	void BindForReading ();
	void BindForWriting ();
	std::vector<PipelineAttribute> GetCustomAttributes () const;
};

#endif