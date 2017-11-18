#ifndef REFLECTIVESHADOWMAPRENDERPASS_H
#define REFLECTIVESHADOWMAPRENDERPASS_H

#include "Renderer/RenderPassI.h"

class ReflectiveShadowMapRenderPass : public RenderPassI
{
public:
	virtual ~ReflectiveShadowMapRenderPass ();

	virtual void Init ();
	virtual RenderVolumeCollection* Execute (Scene* scene, Camera* camera, RenderVolumeCollection* rvc);
protected:
	void StartFrame ();
	void ReflectiveShadowMapTestPass (Camera*, RenderVolumeCollection*);
};

#endif