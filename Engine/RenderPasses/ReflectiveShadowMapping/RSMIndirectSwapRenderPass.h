#ifndef RSMINDIRECTSWAPRENDERPASS_H
#define RSMINDIRECTSWAPRENDERPASS_H

#include "RenderPasses/Container/ContainerRenderSubPassI.h"

class RSMIndirectSwapRenderPass : public ContainerRenderSubPassI
{
	DECLARE_RENDER_PASS(RSMIndirectSwapRenderPass)

public:
	void Init (const RenderSettings& settings);
	RenderVolumeCollection* Execute (const RenderScene* renderScene, const Camera* camera,
		const RenderSettings& settings, RenderVolumeCollection* rvc);

	void Clear ();

	bool IsAvailable (const RenderScene* renderScene, const Camera* camera,
		const RenderSettings& settings, const RenderVolumeCollection* rvc) const;
};

#endif