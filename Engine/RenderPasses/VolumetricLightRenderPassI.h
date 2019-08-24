#ifndef VOLUMETRICLIGHTRENDERPASSI_H
#define VOLUMETRICLIGHTRENDERPASSI_H

#include "Container/ContainerRenderSubPassI.h"

#include "Renderer/RenderLightObject.h"

class ENGINE_API VolumetricLightRenderPassI : public ContainerRenderSubPassI
{
protected:
	RenderLightObject* GetRenderLightObject (const RenderVolumeCollection* rvc) const;
	bool IsAvailable (const RenderScene*, const Camera*,
		const RenderSettings& settings, const RenderVolumeCollection*) const;

	virtual bool IsAvailable (const RenderLightObject*) const = 0;
};

#endif