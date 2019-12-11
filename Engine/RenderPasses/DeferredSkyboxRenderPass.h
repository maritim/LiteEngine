#ifndef DEFERREDSKYBOXRENDERPASS_H
#define DEFERREDSKYBOXRENDERPASS_H

#include "RenderPasses/Container/ContainerRenderSubPassI.h"

class ENGINE_API DeferredSkyboxRenderPass : public ContainerRenderSubPassI
{
	DECLARE_RENDER_PASS(DeferredSkyboxRenderPass)

public:
	virtual ~DeferredSkyboxRenderPass ();

	virtual void Init (const RenderSettings& settings);
	virtual RenderVolumeCollection* Execute (const RenderScene* renderScene, const Camera* camera,
		const RenderSettings& settings, RenderVolumeCollection* rvc);
	virtual bool IsAvailable (const RenderScene* renderScene, const Camera* camera,
		const RenderSettings& settings, const RenderVolumeCollection* rvc) const;

	void Clear ();
protected:
	void StartSkyboxPass (RenderVolumeCollection* rvc);
	void SkyboxPass (const RenderScene* renderScene, const Camera* camera,
		const RenderSettings& settings, RenderVolumeCollection* rvc);
};

#endif