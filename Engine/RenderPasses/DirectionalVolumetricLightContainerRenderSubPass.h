#ifndef DIRECTIONALVOLUMETRICLIGHTRENDERCONTAINERSUBPASS_H
#define DIRECTIONALVOLUMETRICLIGHTRENDERCONTAINERSUBPASS_H

#include "RenderPasses/VolumetricLightContainerRenderSubPassI.h"

class DirectionalVolumetricLightContainerRenderSubPass : public VolumetricLightContainerRenderSubPassI
{
public:
	~DirectionalVolumetricLightContainerRenderSubPass ();

	RenderVolumeCollection* Execute (const Scene*, const Camera*, RenderVolumeCollection* );
protected:
	bool IsAvailable (const VolumetricLight*) const;

	void StartDirectionalLightPass (RenderVolumeCollection*);
	void DirectionalLightPass (const Scene*, const Camera*, RenderVolumeCollection*);
	void EndDirectionalLightPass ();

	virtual void LockShader (const VolumetricLight*) = 0;
};

#endif