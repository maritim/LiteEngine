#ifndef TRSMINDIRECTSWAPRENDERPASS_H
#define TRSMINDIRECTSWAPRENDERPASS_H

#include "RenderPasses/Container/ContainerRenderSubPassI.h"

class TRSMIndirectSwapRenderPass : public ContainerRenderSubPassI
{
	DECLARE_RENDER_PASS(TRSMIndirectSwapRenderPass)

public:
	void Init (const RenderSettings& settings);
	RenderVolumeCollection* Execute (const RenderScene* renderScene, const Camera* camera,
		const RenderSettings& settings, RenderVolumeCollection* rvc);

	void Clear ();

	bool IsAvailable (const RenderScene* renderScene, const Camera* camera,
		const RenderSettings& settings, const RenderVolumeCollection* rvc) const;
};

#endif