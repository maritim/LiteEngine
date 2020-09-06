#ifndef IDLERENDERPASS_H
#define IDLERENDERPASS_H

#include "Container/ContainerRenderSubPassI.h"

#include "RenderPasses/FramebufferRenderVolume.h"

class ENGINE_API IdleRenderPass : public ContainerRenderSubPassI
{
	DECLARE_RENDER_PASS(IdleRenderPass)

protected:
	FramebufferRenderVolume* _framebufferRenderVolume;

public:
	IdleRenderPass ();

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