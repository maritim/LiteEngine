#ifndef DEFERREDBLITRENDERPASS_H
#define DEFERREDBLITRENDERPASS_H

#include "Renderer/RenderPassI.h"

#include "RenderPasses/FrameBuffer2DVolume.h"

class DeferredBlitRenderPass : public RenderPassI
{
public:
	virtual ~DeferredBlitRenderPass ();

	virtual void Init ();
	virtual RenderVolumeCollection* Execute (const Scene* scene, const Camera* camera, RenderVolumeCollection* rvc);

	void Clear ();
protected:
	void EndDrawing (FrameBuffer2DVolume* frameBufferVolume);
};

#endif