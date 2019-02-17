#ifndef IDLECONTAINERRENDERSUBPASS_H
#define IDLECONTAINERRENDERSUBPASS_H

#include "Container/ContainerRenderSubPassI.h"

#include "RenderPasses/PostProcess/PostProcessMapVolume.h"

class IdleContainerRenderSubPass : public ContainerRenderSubPassI
{
protected:
	PostProcessMapVolume* _postProcessMapVolume;

public:
	IdleContainerRenderSubPass ();
	~IdleContainerRenderSubPass ();

	void Init ();
	RenderVolumeCollection* Execute (const Scene* scene, const Camera* camera, RenderVolumeCollection* rvc);

	bool IsAvailable (const Scene* scene, const Camera* camera, const RenderVolumeCollection* rvc) const;

	void Clear ();
};

#endif