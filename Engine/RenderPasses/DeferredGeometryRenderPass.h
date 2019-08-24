#ifndef DEFERREGEOMETRYRENDERPASS_H
#define DEFERREGEOMTRYRENDERPASS_H

#include "RenderPasses/Container/ContainerRenderSubPassI.h"

#include <string>

#include "GBuffer.h"

class ENGINE_API DeferredGeometryRenderPass : public ContainerRenderSubPassI
{
	DECLARE_RENDER_PASS(DeferredGeometryRenderPass)

protected:
	std::string _shaderName;
	std::string _normalMapShaderName;
	std::string _lightMapShaderName;
	std::string _animationShaderName;
	GBuffer* _frameBuffer;

public:
	DeferredGeometryRenderPass ();
	virtual ~DeferredGeometryRenderPass ();

	virtual void Init (const RenderSettings& settings);
	virtual RenderVolumeCollection* Execute (const RenderScene* renderScene, const Camera* camera,
		const RenderSettings& settings, RenderVolumeCollection* rvc);
	virtual bool IsAvailable (const RenderScene* renderScene, const Camera* camera,
		const RenderSettings& settings, const RenderVolumeCollection* rvc) const;

	void Clear ();
protected:
	void UpdateCamera (const Camera* camera);

	void PrepareDrawing ();
	void GeometryPass (const RenderScene* renderScene, const Camera* camera, const RenderSettings& settings);
	void EndDrawing ();

	void LockShader (int sceneLayers);

	void UpdateVolumes (const RenderSettings& settings, RenderVolumeCollection* rvc);

	void InitGBufferVolume (const RenderSettings& settings);
};

#endif