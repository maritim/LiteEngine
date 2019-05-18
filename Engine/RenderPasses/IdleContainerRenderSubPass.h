#ifndef IDLECONTAINERRENDERSUBPASS_H
#define IDLECONTAINERRENDERSUBPASS_H

#include "Container/ContainerRenderSubPassI.h"

#include "RenderPasses/PostProcess/PostProcessMapVolume.h"

class ENGINE_API IdleContainerRenderSubPass : public ContainerRenderSubPassI
{
protected:
	PostProcessMapVolume* _postProcessMapVolume;

public:
	IdleContainerRenderSubPass ();
	~IdleContainerRenderSubPass ();

	void Init (const RenderSettings& settings);
	RenderVolumeCollection* Execute (const Scene* scene, const Camera* camera,
		const RenderSettings& settings, RenderVolumeCollection* rvc);

	bool IsAvailable (const Scene* scene, const Camera* camera,
		const RenderSettings& settings, const RenderVolumeCollection* rvc) const;

	void Clear ();
protected:
	void UpdateVolume (const RenderSettings& settings);

	void InitVolume (const RenderSettings& settings);	
};

#endif