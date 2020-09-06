#ifndef NOISEGENERATIONRENDERPASS_H
#define NOISEGENERATIONRENDERPASS_H

#include "RenderPasses/Container/ContainerRenderSubPassI.h"

#include "RenderPasses/TextureRenderVolume.h"

class ENGINE_API NoiseGenerationRenderPass : public ContainerRenderSubPassI
{
protected:
	TextureRenderVolume* _noiseMapVolume;

public:
	NoiseGenerationRenderPass ();

	void Init (const RenderSettings& settings);
	RenderVolumeCollection* Execute (const RenderScene* renderScene, const Camera* camera,
		const RenderSettings& settings, RenderVolumeCollection* rvc);

	void Clear ();
protected:
	void UpdateNoiseMapVolume (const RenderSettings& settings);

	virtual glm::ivec2 GetNoiseMapVolumeResolution (const RenderSettings& settings) const = 0;
	virtual TextureRenderVolume* CreateNoiseMapVolume (const RenderSettings& settings) const = 0;
};

#endif
