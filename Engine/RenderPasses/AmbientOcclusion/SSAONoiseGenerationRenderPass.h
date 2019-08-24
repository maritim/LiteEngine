#ifndef SSAONOISEGENERATIONRENDERPASS_H
#define SSAONOISESGENERATIONRENDERPASS_H

#include "RenderPasses/Container/ContainerRenderSubPassI.h"

#include "SSAONoiseMapVolume.h"

#define SSAO_NOISE_NOT_INIT 352

class ENGINE_API SSAONoiseGenerationRenderPass : public ContainerRenderSubPassI
{
	DECLARE_RENDER_PASS(SSAONoiseGenerationRenderPass)

protected:
	SSAONoiseMapVolume* _ssaoNoiseMapVolume;

public:
	SSAONoiseGenerationRenderPass ();
	~SSAONoiseGenerationRenderPass ();

	void Init (const RenderSettings& settings);
	RenderVolumeCollection* Execute (const RenderScene* renderScene, const Camera* camera,
		const RenderSettings& settings, RenderVolumeCollection* rvc);

	bool IsAvailable (const RenderScene* renderScene, const Camera* camera,
		const RenderSettings& settings, const RenderVolumeCollection* rvc) const;

	void Clear ();
protected:
	void InitNoiseMapVolume (const RenderSettings& settings);

	void UpdateNoiseMapVolume (const RenderSettings& settings);
};

#endif
