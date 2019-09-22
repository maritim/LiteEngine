#ifndef NOISEGENERATIONRENDERPASS_H
#define NOISEGENERATIONRENDERPASS_H

#include "RenderPasses/Container/ContainerRenderSubPassI.h"

#include "NoiseMapVolume.h"

class ENGINE_API NoiseGenerationRenderPass : public ContainerRenderSubPassI
{
	DECLARE_RENDER_PASS(NoiseGenerationRenderPass)

protected:
	NoiseMapVolume* _noiseMapVolume;

public:
	NoiseGenerationRenderPass ();
	~NoiseGenerationRenderPass ();

	void Init (const RenderSettings& settings);
	RenderVolumeCollection* Execute (const RenderScene* renderScene, const Camera* camera,
		const RenderSettings& settings, RenderVolumeCollection* rvc);

	void Clear ();
protected:
	void InitNoiseMapVolume (const RenderSettings& settings);

	void UpdateNoiseMapVolume (const RenderSettings& settings);

	virtual glm::ivec2 GetNoiseMapVolumeResolution (const RenderSettings& settings) const = 0;
};

#endif
