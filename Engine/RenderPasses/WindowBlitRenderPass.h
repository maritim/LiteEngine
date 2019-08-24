#ifndef WINDOWBLITRENDERPASS_H
#define WINDOWBLITRENDERPASS_H

#include "Renderer/RenderPassI.h"

#include "RenderPasses/FrameBuffer2DVolume.h"

class WindowBlitRenderPass : public RenderPassI
{
	DECLARE_RENDER_PASS(WindowBlitRenderPass)

public:
	virtual ~WindowBlitRenderPass ();

	virtual void Init (const RenderSettings& settings);
	virtual RenderVolumeCollection* Execute (const RenderScene* renderScene, const Camera* camera,
		const RenderSettings& settings, RenderVolumeCollection* rvc);

	void Clear ();
protected:
	void EndDrawing (FrameBuffer2DVolume* frameBufferVolume, const RenderSettings& settings);
};

#endif