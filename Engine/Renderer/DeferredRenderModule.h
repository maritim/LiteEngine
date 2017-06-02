#ifndef DEFERREDRENDERMODULE_H
#define DEFERREDRENDERMODULE_H

#include "RenderModule.h"

#include "Lighting/VolumetricLight.h"

#include "GBuffer.h"

class DeferredRenderModule : virtual public RenderModule
{
protected:
	GBuffer* _frameBuffer;

public:
	DeferredRenderModule ();
	virtual ~DeferredRenderModule ();

	virtual void RenderScene (Scene*, Camera*);
protected:
	void UpdateCamera (Camera* camera);
	void UpdateGBuffer ();

	void DeferredPass (Scene*, Camera* camera);
	void ForwardPass (Scene*);

	void PrepareDrawing ();
	void GeometryPass (Scene* scene, Camera* camera);
	void LightPass (Scene* scene, Camera* camera);
	void SkyboxPass (Camera* camera);
	void EndDrawing ();

	void DirectionalLightPass (Scene* scene, Camera* camera);
	void PointLightPass (Scene* scene, Camera* camera);

	void DirectionalLightShadowPass (VolumetricLight* light, Scene* scene, Camera* camera);

	void PointLightStencilPass (VolumetricLight* light);
	void PointLightDrawPass (VolumetricLight* light);
};

#endif