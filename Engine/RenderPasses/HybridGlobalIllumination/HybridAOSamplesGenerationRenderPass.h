#ifndef HYBRIDAOSAMPLESGENERATIONRENDERPASS_H
#define HYBRIDAOSAMPLESGENERATIONRENDERPASS_H

#include "RenderPasses/ScreenSpaceAmbientOcclusion/SSAOSamplesGenerationRenderPass.h"

class ENGINE_API HybridAOSamplesGenerationRenderPass : public SSAOSamplesGenerationRenderPass
{
	DECLARE_RENDER_PASS(HybridAOSamplesGenerationRenderPass)

public:
	virtual void Init (const RenderSettings& settings);

	bool IsAvailable (const RenderScene* renderScene, const Camera* camera,
		const RenderSettings& settings, const RenderVolumeCollection* rvc) const;
protected:
	void UpdateSamplesVolume (const RenderSettings& settings);
};

#endif
