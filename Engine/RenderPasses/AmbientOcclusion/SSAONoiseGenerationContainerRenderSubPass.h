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

	void Init (const RenderSettings& settings);
	RenderVolumeCollection* Execute (const Scene* scene, const Camera* camera,
		const RenderSettings& settings, RenderVolumeCollection* rvc);

	bool IsAvailable (const Scene* scene, const Camera* camera,
		const RenderSettings& settings, const RenderVolumeCollection* rvc) const;

	void Clear ();
protected:
	void InitNoiseMapVolume (const RenderSettings& settings);

	void UpdateNoiseMapVolume (const RenderSettings& settings);
};

#endif
