#ifndef WINDOWBLITRENDERPASS_H
#define WINDOWBLITRENDERPASS_H

#include "Renderer/RenderPassI.h"

#include "RenderPasses/FrameBuffer2DVolume.h"

class WindowBlitRenderPass : public RenderPassI
{
public:
	virtual ~WindowBlitRenderPass ();

	virtual void Init (const RenderSettings& settings);
	virtual RenderVolumeCollection* Execute (const Scene* scene, const Camera* camera,
		const RenderSettings& settings, RenderVolumeCollection* rvc);

	void Clear ();
protected:
	void EndDrawing (FrameBuffer2DVolume* frameBufferVolume, const RenderSettings& settings);
};

#endif