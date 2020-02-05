#ifndef DEFERREGEOMETRYRENDERPASS_H
#define DEFERREGEOMTRYRENDERPASS_H

#include "RenderPasses/Container/ContainerRenderSubPassI.h"

#include "Core/Resources/Resource.h"
#include "Renderer/RenderViews/ShaderView.h"

#include "GBuffer.h"

#include "Utils/Sequences/HaltonGenerator.h"

class ENGINE_API DeferredGeometryRenderPass : public ContainerRenderSubPassI
{
	DECLARE_RENDER_PASS(DeferredGeometryRenderPass)

protected:
	Resource<ShaderView> _shaderView;
	Resource<ShaderView> _normalMapShaderView;
	Resource<ShaderView> _lightMapShaderView;
	Resource<ShaderView> _animationShaderView;
	GBuffer* _frameBuffer;
	HaltonGenerator _haltonGenerator;

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
	void UpdateCamera (const Camera* camera, const RenderSettings& settings);

	void PrepareDrawing ();
	void GeometryPass (const RenderScene* renderScene, const Camera* camera, const RenderSettings& settings);
	void EndDrawing ();

	void LockShader (int sceneLayers);

	void UpdateVolumes (const RenderSettings& settings, RenderVolumeCollection* rvc);

	void InitGBufferVolume (const RenderSettings& settings);
};

#endif