#ifndef CONTAINERRENDERSUBPASS_H
#define CONTAINERRENDERSUBPASS_H

#include "Renderer/RenderPassI.h"

class ENGINE_API ContainerRenderSubPassI : public RenderPassI
{
public:
	virtual bool IsAvailable (const RenderScene* renderScene, const Camera* camera,
		const RenderSettings& settings, const RenderVolumeCollection* rvc) const = 0;
};

#endif