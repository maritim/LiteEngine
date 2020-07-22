#ifndef TAASWAPRENDERPASS_H
#define TAASWAPRENDERPASS_H

#include "RenderPasses/TemporalFiltering/TemporalFilterSwapRenderPass.h"

class ENGINE_API TAASwapRenderPass : public TemporalFilterSwapRenderPass
{
	DECLARE_RENDER_PASS(TAASwapRenderPass)

public:
	bool IsAvailable (const RenderScene* renderScene, const Camera* camera,
		const RenderSettings& settings, const RenderVolumeCollection* rvc) const;
};

#endif