#ifndef SHADOWMAPVOLUME_H
#define SHADOWMAPVOLUME_H

#include "RenderPasses/FrameBuffer2DVolume.h"

class ShadowMapVolume : public FrameBuffer2DVolume
{
public:
	bool Init (std::size_t bufferWidth, std::size_t bufferHeight);

	void BindForReading ();
	void BindForWriting ();

	std::vector<PipelineAttribute> GetCustomAttributes () const;
};

#endif