#ifndef DEFERREDSKYBOXRENDERPASS_H
#define DEFERREDSKYBOXRENDERPASS_H

#include "Renderer/RenderPassI.h"

class DeferredSkyboxRenderPass : public RenderPassI
{
public:
	virtual ~DeferredSkyboxRenderPass ();

	virtual void Init ();
	virtual RenderVolumeCollection* Execute (const Scene* scene, const Camera* camera, RenderVolumeCollection* rvc);
protected:
	void SkyboxPass (const Scene* scene, const Camera* camera);
};

#endif