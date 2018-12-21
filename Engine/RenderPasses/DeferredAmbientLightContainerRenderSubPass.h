#ifndef DEFERREDAMBIENTLIGHTRENDERCONTAINERSUBPASS_H
#define DEFERREDAMBIENTLIGHTRENDERCONTAINERSUBPASS_H

#include "RenderPasses/Container/ContainerRenderSubPassI.h"

#include <string>

class DeferredAmbientLightContainerRenderSubPass : public ContainerRenderSubPassI
{
protected:
	std::string _shaderName;

public:
	DeferredAmbientLightContainerRenderSubPass ();
	virtual ~DeferredAmbientLightContainerRenderSubPass ();

	virtual void Init ();
	virtual RenderVolumeCollection* Execute (const Scene* scene, const Camera* camera, RenderVolumeCollection* rvc);

	virtual bool IsAvailable (const Scene* scene, const Camera* camera, const RenderVolumeCollection* rvc) const;
protected:
	void StartAmbientLightPass (RenderVolumeCollection* rvc);
	void AmbientLightPass (const Scene* scene, const Camera* camera, RenderVolumeCollection* rvc);
	void EndAmbientLightPass ();

	void LockShader ();
	std::vector<PipelineAttribute> GetCustomAttributes (RenderVolumeCollection* rvc);
};

#endif