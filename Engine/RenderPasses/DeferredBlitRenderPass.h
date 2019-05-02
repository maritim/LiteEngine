#ifndef DEFERREDBLITRENDERPASS_H
#define DEFERREDBLITRENDERPASS_H

#include "Renderer/RenderPassI.h"

#include "RenderPasses/FrameBuffer2DVolume.h"

class DeferredBlitRenderPass : public RenderPassI
{
public:
	virtual ~DeferredBlitRenderPass ();

	virtual void Init (const RenderSettings& settings);
	virtual RenderVolumeCollection* Execute (const Scene* scene, const Camera* camera,
		const RenderSettings& settings, RenderVolumeCollection* rvc);

	void Clear ();
protected:
	void EndDrawing (FrameBuffer2DVolume* frameBufferVolume, FrameBuffer2DVolume* resultFramebuffer,
		const RenderSettings& settings);
};

#endif