#ifndef DEFERREGEOMETRYRENDERPASS_H
#define DEFERREGEOMTRYRENDERPASS_H

#include "Renderer/RenderPassI.h"

#include "GBuffer.h"

class DeferredGeometryRenderPass : public RenderPassI
{
protected:
	GBuffer* _frameBuffer;

public:
	DeferredGeometryRenderPass ();
	virtual ~DeferredGeometryRenderPass ();

	virtual void Init ();
	virtual RenderVolumeCollection* Execute (Scene* scene, Camera* camera, RenderVolumeCollection* rvc);
protected:
	void UpdateCamera (Camera* camera);
	void UpdateGBuffer ();

	void PrepareDrawing ();
	void GeometryPass (Scene* scene, Camera* camera);
};

#endif