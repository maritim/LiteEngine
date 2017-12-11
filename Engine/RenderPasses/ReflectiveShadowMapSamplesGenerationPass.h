#ifndef REFLECTIVESHADOWMAPSAMPLESGENERATIONPASS_H
#define REFLECTIVESHADOWMAPSAMPLESGENERATIONPASS_H

#include "Renderer/RenderPassI.h"

#include "ReflectiveShadowMapSamplesVolume.h"

class ReflectiveShadowMapSamplesGenerationPass : public RenderPassI
{
protected:
	ReflectiveShadowMapSamplesVolume* _reflectiveShadowMapSamplesVolume;

public:
	ReflectiveShadowMapSamplesGenerationPass ();

	virtual void Init();
	virtual RenderVolumeCollection* Execute(Scene* scene, Camera* camera, RenderVolumeCollection* rvc);
};

#endif
