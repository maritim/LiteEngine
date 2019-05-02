#ifndef CONTAINERRENDERSUBPASS_H
#define CONTAINERRENDERSUBPASS_H

#include "Renderer/RenderPassI.h"

class ContainerRenderSubPassI : public RenderPassI
{
public:
	virtual bool IsAvailable (const Scene* scene, const Camera* camera,
		const RenderSettings& settings, const RenderVolumeCollection* rvc) const = 0;
};

#endif