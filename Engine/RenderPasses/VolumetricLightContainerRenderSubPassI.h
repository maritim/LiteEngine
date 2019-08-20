#ifndef DEFERREDVOLUMETRICLIGHTRENDERPASS_H
#define DEFERREDVOLUMETRICLIGHTRENDERPASS_H

#include "Container/ContainerRenderSubPassI.h"

#include "Renderer/RenderLightObject.h"

class ENGINE_API VolumetricLightContainerRenderSubPassI : public ContainerRenderSubPassI
{
protected:
	RenderLightObject* GetRenderLightObject (const RenderVolumeCollection* rvc) const;
	bool IsAvailable (const RenderScene*, const Camera*,
		const RenderSettings& settings, const RenderVolumeCollection*) const;

	virtual bool IsAvailable (const RenderLightObject*) const = 0;
};

#endif