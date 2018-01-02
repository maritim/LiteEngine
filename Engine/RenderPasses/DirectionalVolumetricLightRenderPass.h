#ifndef DIRECTIONALVOLUMETRICLIGHTRENDERPASS_H
#define DIRECTIONALVOLUMETRICLIGHTRENDERPASS_H

#include "VolumetricLightContainerRenderSubPassI.h"

class DirectionalVolumetricLightRenderPass : public VolumetricLightContainerRenderSubPassI
{
public:
	~DirectionalVolumetricLightRenderPass ();

	RenderVolumeCollection* Execute (const Scene*, const Camera*, RenderVolumeCollection* );
protected:
	bool IsAvailable (const VolumetricLight*) const;

	void DirectionalLightPass (const Scene*, const Camera*, RenderVolumeCollection*);
	void EndDirectionalLightPass ();

	virtual void LockShader (const VolumetricLight*) = 0;
};

#endif