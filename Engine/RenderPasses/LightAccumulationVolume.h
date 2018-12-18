#ifndef LIGHTACCUMULATIONVOLUME_H
#define LIGHTACCUMULATIONVOLUME_H

#include "RenderPasses/FrameBuffer2DVolume.h"

#include "GBuffer.h"

#define LIGHT_ACCUMULATION_FBO_NOT_INIT 315

class LightAccumulationVolume : public FrameBuffer2DVolume
{
protected:
	unsigned int _depthBuffer;

public:
	LightAccumulationVolume ();
	virtual ~LightAccumulationVolume ();

	virtual bool Init (std::size_t screenWidth, std::size_t screenHeight, GBuffer* gBuffer);

	void BindForReading ();
	void BindForWriting ();
	std::vector<PipelineAttribute> GetCustomAttributes () const;
};

#endif