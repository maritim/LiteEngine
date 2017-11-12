#ifndef DEFERREDLIGHTRENDERPASS_H
#define DEFERREDLIGHTRENDERPASS_H

#include "Renderer/RenderPassI.h"

#include "Lighting/VolumetricLight.h"

class DeferredLightRenderPass : public RenderPassI
{
public:
	virtual ~DeferredLightRenderPass ();

	virtual void Init ();
	virtual RenderVolumeCollection* Execute (Scene* scene, Camera* camera, RenderVolumeCollection* rvc);
protected:
	void LightPass (Scene* scene, Camera* camera, RenderVolumeCollection* rvc);

	void DirectionalLightPass (Scene* scene, Camera* camera, RenderVolumeCollection* );
	void PointLightPass (Scene* scene, Camera* camera, RenderVolumeCollection*);

	void PointLightStencilPass (VolumetricLight* light, RenderVolumeCollection*);
	void PointLightDrawPass (VolumetricLight* light, RenderVolumeCollection*);
};

#endif