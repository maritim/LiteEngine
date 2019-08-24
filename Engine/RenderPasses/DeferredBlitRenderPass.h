#ifndef DEFERREDBLITRENDERPASS_H
#define DEFERREDBLITRENDERPASS_H

#include "RenderPasses/Container/ContainerRenderSubPassI.h"

#include "RenderPasses/FrameBuffer2DVolume.h"

class ENGINE_API DeferredBlitRenderPass : public ContainerRenderSubPassI
{
	DECLARE_RENDER_PASS(DeferredBlitRenderPass)

public:
	virtual ~DeferredBlitRenderPass ();

	virtual void Init (const RenderSettings& settings);
	virtual RenderVolumeCollection* Execute (const RenderScene* renderScene, const Camera* camera,
		const RenderSettings& settings, RenderVolumeCollection* rvc);
	virtual bool IsAvailable (const RenderScene* renderScene, const Camera* camera,
		const RenderSettings& settings, const RenderVolumeCollection* rvc) const;

	void Clear ();
protected:
	void EndDrawing (FrameBuffer2DVolume* frameBufferVolume, FrameBuffer2DVolume* resultFramebuffer,
		const RenderSettings& settings);
};

#endif