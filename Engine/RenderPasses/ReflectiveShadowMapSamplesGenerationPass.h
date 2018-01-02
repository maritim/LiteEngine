#ifndef REFLECTIVESHADOWMAPSAMPLESGENERATIONPASS_H
#define REFLECTIVESHADOWMAPSAMPLESGENERATIONPASS_H

#include "VolumetricLightContainerRenderSubPassI.h"

#include "ReflectiveShadowMapSamplesVolume.h"

class ReflectiveShadowMapSamplesGenerationPass : public VolumetricLightContainerRenderSubPassI
{
protected:
	ReflectiveShadowMapSamplesVolume* _reflectiveShadowMapSamplesVolume;

public:
	ReflectiveShadowMapSamplesGenerationPass ();

	virtual void Init();
	virtual RenderVolumeCollection* Execute(const Scene* scene, const Camera* camera, RenderVolumeCollection* rvc);

	virtual bool IsAvailable (const VolumetricLight*) const;
};

#endif
