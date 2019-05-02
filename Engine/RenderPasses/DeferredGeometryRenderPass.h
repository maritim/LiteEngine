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

	virtual void Init (const RenderSettings& settings);
	virtual RenderVolumeCollection* Execute (const Scene* scene, const Camera* camera,
		const RenderSettings& settings, RenderVolumeCollection* rvc);

	void Clear ();
protected:
	void UpdateCamera (const Camera* camera);

	void PrepareDrawing ();
	void GeometryPass (const Scene* scene, const Camera* camera, const RenderSettings& settings);
	void EndDrawing ();

	void LockShader (int sceneLayers);

	void UpdateVolumes (const RenderSettings& settings);

	void InitGBufferVolume (const RenderSettings& settings);
	void InitLightAccumulationVolume (const RenderSettings& settings);
};

#endif