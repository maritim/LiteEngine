#ifndef SSAOSAMPLESGENERATIONRENDERPASS_H
#define SSAOSAMPLESGENERATIONRENDERPASS_H

#include "RenderPasses/Container/ContainerRenderSubPassI.h"

#include "SSAOSamplesVolume.h"

class ENGINE_API SSAOSamplesGenerationRenderPass : public ContainerRenderSubPassI
{
	DECLARE_RENDER_PASS(SSAOSamplesGenerationRenderPass)

protected:
	SSAOSamplesVolume* _ssaoSamplesVolume;

public:
	SSAOSamplesGenerationRenderPass ();

	virtual void Init (const RenderSettings& settings);
	RenderVolumeCollection* Execute (const RenderScene* renderScene, const Camera* camera,
		const RenderSettings& settings, RenderVolumeCollection* rvc);

	bool IsAvailable (const RenderScene* renderScene, const Camera* camera,
		const RenderSettings& settings, const RenderVolumeCollection* rvc) const;

	void Clear ();
protected:
	virtual void UpdateSamplesVolume (const RenderSettings& settings);
};

#endif
