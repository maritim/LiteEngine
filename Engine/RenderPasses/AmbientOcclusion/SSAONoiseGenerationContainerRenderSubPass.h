#ifndef SSAONOISEGENERATIONCONTAINERRENDERSUBPASS_H
#define SSAONOISESGENERATIONCONTAINERRENDERSUBPASS_H

#include "RenderPasses/Container/ContainerRenderSubPassI.h"

#include "SSAONoiseMapVolume.h"

#define SSAO_NOISE_NOT_INIT 352

class SSAONoiseGenerationContainerRenderSubPass : public ContainerRenderSubPassI
{
protected:
	SSAONoiseMapVolume* _ssaoNoiseMapVolume;

public:
	SSAONoiseGenerationContainerRenderSubPass ();
	~SSAONoiseGenerationContainerRenderSubPass ();

	void Init ();
	RenderVolumeCollection* Execute (const Scene* scene, const Camera* camera, RenderVolumeCollection* rvc);

	bool IsAvailable (const Scene* scene, const Camera* camera, const RenderVolumeCollection* rvc) const;
};

#endif
