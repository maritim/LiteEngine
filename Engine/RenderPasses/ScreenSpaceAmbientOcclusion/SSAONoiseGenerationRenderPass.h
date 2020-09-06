#ifndef SSAONOISEGENERATIONRENDERPASS_H
#define SSAONOISEGENERATIONRENDERPASS_H

#include "RenderPasses/NoiseMap/NoiseGenerationRenderPass.h"

class ENGINE_API SSAONoiseGenerationRenderPass : public NoiseGenerationRenderPass
{
	DECLARE_RENDER_PASS(SSAONoiseGenerationRenderPass)

public:
	bool IsAvailable (const RenderScene* renderScene, const Camera* camera,
		const RenderSettings& settings, const RenderVolumeCollection* rvc) const;
protected:
	glm::ivec2 GetNoiseMapVolumeResolution (const RenderSettings& settings) const;
	TextureRenderVolume* CreateNoiseMapVolume (const RenderSettings& settings) const;
};

#endif