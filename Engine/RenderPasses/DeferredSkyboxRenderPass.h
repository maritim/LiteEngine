#ifndef DEFERREDSKYBOXRENDERPASS_H
#define DEFERREDSKYBOXRENDERPASS_H

#include "Renderer/RenderPassI.h"

class DeferredSkyboxRenderPass : public RenderPassI
{
public:
	virtual ~DeferredSkyboxRenderPass ();

	virtual void Init ();
	virtual RenderVolumeCollection* Execute (Scene* scene, Camera* camera, RenderVolumeCollection* rvc);
protected:
	void SkyboxPass (Camera* camera);
};

#endif