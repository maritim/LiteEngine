#ifndef TAASWAPRENDERPASS_H
#define TAASWAPRENDERPASS_H

#include "RenderPasses/Container/ContainerRenderSubPassI.h"

class ENGINE_API TAASwapRenderPass : public ContainerRenderSubPassI
{
	DECLARE_RENDER_PASS(TAASwapRenderPass)

public:
	void Init (const RenderSettings& settings);
	RenderVolumeCollection* Execute (const RenderScene* renderScene, const Camera* camera,
		const RenderSettings& settings, RenderVolumeCollection* rvc);

	void Clear ();

	bool IsAvailable (const RenderScene* renderScene, const Camera* camera,
		const RenderSettings& settings, const RenderVolumeCollection* rvc) const;
};

#endif