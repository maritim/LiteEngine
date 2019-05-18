#ifndef DEFERREDAMBIENTLIGHTRENDERCONTAINERSUBPASS_H
#define DEFERREDAMBIENTLIGHTRENDERCONTAINERSUBPASS_H

#include "RenderPasses/Container/ContainerRenderSubPassI.h"

#include <string>

class ENGINE_API DeferredAmbientLightContainerRenderSubPass : public ContainerRenderSubPassI
{
protected:
	std::string _shaderName;
	std::string _aoShaderName;

public:
	DeferredAmbientLightContainerRenderSubPass ();
	virtual ~DeferredAmbientLightContainerRenderSubPass ();

	virtual void Init (const RenderSettings& settings);
	virtual RenderVolumeCollection* Execute (const Scene* scene, const Camera* camera,
		const RenderSettings& settings, RenderVolumeCollection* rvc);

	virtual bool IsAvailable (const Scene* scene, const Camera* camera,
		const RenderSettings& settings, const RenderVolumeCollection* rvc) const;

	void Clear ();
protected:
	void StartAmbientLightPass (const RenderSettings& settings, RenderVolumeCollection* rvc);
	void AmbientLightPass (const Scene* scene, const Camera* camera,
		const RenderSettings& settings, RenderVolumeCollection* rvc);
	void EndAmbientLightPass ();

	void LockShader (const RenderSettings& settings);
	std::vector<PipelineAttribute> GetCustomAttributes (RenderVolumeCollection* rvc);
};

#endif