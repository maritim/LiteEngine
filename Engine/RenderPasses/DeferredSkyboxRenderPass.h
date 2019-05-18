#ifndef DEFERREDSKYBOXRENDERPASS_H
#define DEFERREDSKYBOXRENDERPASS_H

#include "Renderer/RenderPassI.h"

class ENGINE_API DeferredSkyboxRenderPass : public RenderPassI
{
public:
	virtual ~DeferredSkyboxRenderPass ();

	virtual void Init (const RenderSettings& settings);
	virtual RenderVolumeCollection* Execute (const Scene* scene, const Camera* camera,
		const RenderSettings& settings, RenderVolumeCollection* rvc);

	void Clear ();
protected:
	void StartSkyboxPass (RenderVolumeCollection* rvc);
	void SkyboxPass (const Scene* scene, const Camera* camera);
};

#endif