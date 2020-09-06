#ifndef RESULTFRAMEBUFFERGENERATIONRENDERPASS_H
#define RESULTFRAMEBUFFERGENERATIONRENDERPASS_H

#include "Container/ContainerRenderSubPassI.h"

#include "FramebufferRenderVolume.h"

class ENGINE_API ResultFrameBufferGenerationRenderPass : public ContainerRenderSubPassI
{
	DECLARE_RENDER_PASS(ResultFrameBufferGenerationRenderPass)

protected:
	FramebufferRenderVolume* _resultVolume;

public:
	ResultFrameBufferGenerationRenderPass ();

	void Init (const RenderSettings& settings);
	RenderVolumeCollection* Execute (const RenderScene* renderScene, const Camera* camera,
		const RenderSettings& settings, RenderVolumeCollection* rvc);

	bool IsAvailable (const RenderScene* renderScene, const Camera* camera,
		const RenderSettings& settings, const RenderVolumeCollection* rvc) const;

	void Clear ();
protected:
	void UpdateVolume (const RenderSettings& settings);

	void InitVolume (const RenderSettings& settings);	
};

#endif