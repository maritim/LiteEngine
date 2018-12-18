#ifndef DEFERREGEOMETRYRENDERPASS_H
#define DEFERREGEOMTRYRENDERPASS_H

#include "Renderer/RenderPassI.h"

#include <string>

#include "GBuffer.h"
#include "LightAccumulationVolume.h"

class DeferredGeometryRenderPass : public RenderPassI
{
protected:
	std::string _shaderName;
	std::string _normalMapShaderName;
	std::string _lightMapShaderName;
	std::string _animationShaderName;
	GBuffer* _frameBuffer;
	LightAccumulationVolume* _lightAccumulationVolume;

public:
	DeferredGeometryRenderPass ();
	virtual ~DeferredGeometryRenderPass ();

	virtual void Init ();
	virtual RenderVolumeCollection* Execute (const Scene* scene, const Camera* camera, RenderVolumeCollection* rvc);
protected:
	void UpdateCamera (const Camera* camera);

	void PrepareDrawing ();
	void GeometryPass (const Scene* scene, const Camera* camera);
	void EndDrawing ();

	void LockShader (int sceneLayers);
};

#endif