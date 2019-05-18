#ifndef SSAOSAMPLESGENERATIONCONTAINERRENDERSUBPASS_H
#define SSAOSAMPLESGENERATIONCONTAINERRENDERSUBPASS_H

#include "RenderPasses/Container/ContainerRenderSubPassI.h"

#include "SSAOSamplesVolume.h"

class ENGINE_API SSAOSamplesGenerationContainerRenderSubPass : public ContainerRenderSubPassI
{
protected:
	SSAOSamplesVolume* _ssaoSamplesVolume;

public:
	SSAOSamplesGenerationContainerRenderSubPass ();
	virtual ~SSAOSamplesGenerationContainerRenderSubPass ();

	virtual void Init (const RenderSettings& settings);
	RenderVolumeCollection* Execute (const Scene* scene, const Camera* camera,
		const RenderSettings& settings, RenderVolumeCollection* rvc);

	bool IsAvailable (const Scene* scene, const Camera* camera,
		const RenderSettings& settings, const RenderVolumeCollection* rvc) const;

	void Clear ();
protected:
	void UpdateSamplesVolume (const RenderSettings& settings);

	void InitSamplesVolume (const RenderSettings& settings);
};

#endif
