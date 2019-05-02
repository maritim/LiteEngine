#ifndef DEFERREDVOLUMETRICLIGHTRENDERPASS_H
#define DEFERREDVOLUMETRICLIGHTRENDERPASS_H

#include "Container/ContainerRenderSubPassI.h"

#include "Lighting/VolumetricLight.h"

class VolumetricLightContainerRenderSubPassI : public ContainerRenderSubPassI
{
protected:
	VolumetricLight* GetVolumetricLight (const RenderVolumeCollection* rvc) const;
	bool IsAvailable (const Scene*, const Camera*,
		const RenderSettings& settings, const RenderVolumeCollection*) const;

	virtual bool IsAvailable (const VolumetricLight*) const = 0;
};

#endif