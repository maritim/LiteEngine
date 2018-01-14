#ifndef REFLECTIVESHADOWMAPSAMPLESGENERATIONCONTAINERRENDERSUBPASS_H
#define REFLECTIVESHADOWMAPSAMPLESGENERATIONCONTAINERRENDERSUBPASS_H

#include "RenderPasses/VolumetricLightContainerRenderSubPassI.h"

#include "ReflectiveShadowMapSamplesVolume.h"

class ReflectiveShadowMapSamplesGenerationContainerRenderSubPass : public VolumetricLightContainerRenderSubPassI
{
protected:
	ReflectiveShadowMapSamplesVolume* _reflectiveShadowMapSamplesVolume;

public:
	ReflectiveShadowMapSamplesGenerationContainerRenderSubPass ();

	virtual void Init();
	virtual RenderVolumeCollection* Execute(const Scene* scene, const Camera* camera, RenderVolumeCollection* rvc);

	virtual bool IsAvailable (const VolumetricLight*) const;
};

#endif
