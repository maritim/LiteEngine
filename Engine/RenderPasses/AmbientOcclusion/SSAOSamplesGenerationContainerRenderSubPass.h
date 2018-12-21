#ifndef SSAOSAMPLESGENERATIONCONTAINERRENDERSUBPASS_H
#define SSAOSAMPLESGENERATIONCONTAINERRENDERSUBPASS_H

#include "RenderPasses/Container/ContainerRenderSubPassI.h"

#include "SSAOSamplesVolume.h"

class SSAOSamplesGenerationContainerRenderSubPass : public ContainerRenderSubPassI
{
protected:
	SSAOSamplesVolume* _ssaoSamplesVolume;

public:
	SSAOSamplesGenerationContainerRenderSubPass ();
	virtual ~SSAOSamplesGenerationContainerRenderSubPass ();

	virtual void Init ();
	RenderVolumeCollection* Execute (const Scene* scene, const Camera* camera, RenderVolumeCollection* rvc);

	bool IsAvailable (const Scene* scene, const Camera* camera, const RenderVolumeCollection* rvc) const;
};

#endif
