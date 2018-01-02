#ifndef RENDERCONTAINERSUBPASSI_H
#define RENDERCONTAINERSUBPASSI_H

#include "Renderer/RenderPassI.h"

class RenderContainerSubPassI : public RenderPassI
{
public:
	virtual bool IsAvailable (const Scene* scene, const Camera* camera, const RenderVolumeCollection* rvc) const = 0;
};

#endif