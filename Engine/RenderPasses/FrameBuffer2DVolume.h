#ifndef FRAMEBUFFERVOLUME_H
#define FRAMEBUFFERVOLUME_H

#include "Renderer/RenderVolumeI.h"

class FrameBuffer2DVolume : public RenderVolumeI
{
protected:
	unsigned int _colorBuffer;
	unsigned int _fbo;

public:
	FrameBuffer2DVolume ();
	virtual ~FrameBuffer2DVolume ();

	virtual bool Init (std::size_t bufferWidth, std::size_t bufferHeight);

	void BindForWriting ();
	void BindForBliting ();

	void Clear ();
};

#endif