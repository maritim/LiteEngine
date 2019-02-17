#ifndef POSTPROCESSMAPVOLUME_H
#define POSTPROCESSMAPVOLUME_H

#include "RenderPasses/FrameBuffer2DVolume.h"

#define POST_PROCESS_MAP_VOLUME_NOT_INIT 354

class PostProcessMapVolume : public FrameBuffer2DVolume
{
public:
	virtual void BindForReading ();

	std::vector<PipelineAttribute> GetCustomAttributes () const;
};

#endif