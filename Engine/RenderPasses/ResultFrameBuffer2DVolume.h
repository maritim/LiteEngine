#ifndef RESULTFRAMEBUFFERVOLUME_H
#define RESULTFRAMEBUFFERVOLUME_H

#include "RenderPasses/FrameBuffer2DVolume.h"

#define RESULT_FRAMEBUFFER_FBO_NOT_INIT 315

class ResultFrameBuffer2DVolume : public FrameBuffer2DVolume
{
protected:
	unsigned int _depthBuffer;

public:
	ResultFrameBuffer2DVolume ();

	void AttachDepthBuffer (unsigned int depthBufferID);

	void BindForReading ();
	void BindForWriting ();
	std::vector<PipelineAttribute> GetCustomAttributes () const;

	unsigned int GetDepthBuffer () const;
};

#endif