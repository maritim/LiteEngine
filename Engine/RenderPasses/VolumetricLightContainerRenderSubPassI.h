#ifndef DEFERREDVOLUMETRICLIGHTRENDERPASS_H
#define DEFERREDVOLUMETRICLIGHTRENDERPASS_H

#include "RenderContainerSubPassI.h"

#include "Lighting/VolumetricLight.h"

class VolumetricLightContainerRenderSubPassI : public RenderContainerSubPassI
{
protected:
	VolumetricLight* GetVolumetricLight (const RenderVolumeCollection* rvc) const;
	bool IsAvailable (const Scene*, const Camera*, const RenderVolumeCollection*) const;

	virtual bool IsAvailable (const VolumetricLight*) const = 0;
};

#endif