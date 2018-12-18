#ifndef POSTPROCESSMAPVOLUME_H
#define POSTPROCESSMAPVOLUME_H

#include "RenderPasses/FrameBuffer2DVolume.h"

class PostProcessMapVolume : public FrameBuffer2DVolume
{
public:
	virtual void BindForReading ();

	std::vector<PipelineAttribute> GetCustomAttributes () const;
};

#endif