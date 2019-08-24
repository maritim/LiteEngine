#ifndef DEFERREDAMBIENTLIGHTRENDERPASS_H
#define DEFERREDAMBIENTLIGHTRENDERPASS_H

#include "RenderPasses/Container/ContainerRenderSubPassI.h"

#include <string>

class ENGINE_API DeferredAmbientLightRenderPass : public ContainerRenderSubPassI
{
	DECLARE_RENDER_PASS(DeferredAmbientLightRenderPass)

protected:
	std::string _shaderName;
	std::string _aoShaderName;

public:
	DeferredAmbientLightRenderPass ();
	virtual ~DeferredAmbientLightRenderPass ();

	virtual void Init (const RenderSettings& settings);
	virtual RenderVolumeCollection* Execute (const RenderScene* renderScene, const Camera* camera,
		const RenderSettings& settings, RenderVolumeCollection* rvc);

	virtual bool IsAvailable (const RenderScene* renderScene, const Camera* camera,
		const RenderSettings& settings, const RenderVolumeCollection* rvc) const;

	void Clear ();
protected:
	void StartAmbientLightPass (const RenderSettings& settings, RenderVolumeCollection* rvc);
	void AmbientLightPass (const RenderScene* renderScene, const Camera* camera,
		const RenderSettings& settings, RenderVolumeCollection* rvc);
	void EndAmbientLightPass ();

	void LockShader (const RenderSettings& settings);
	std::vector<PipelineAttribute> GetCustomAttributes (const RenderScene* renderScene, RenderVolumeCollection* rvc);
};

#endif