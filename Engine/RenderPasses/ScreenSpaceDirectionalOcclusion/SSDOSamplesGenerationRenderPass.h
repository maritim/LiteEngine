#ifndef SSADSAMPLESGENERATIONRENDERPASS_H
#define SSADSAMPLESGENERATIONRENDERPASS_H

#include "RenderPasses/Container/ContainerRenderSubPassI.h"

#include "SSDOSamplesVolume.h"

class ENGINE_API SSDOSamplesGenerationRenderPass : public ContainerRenderSubPassI
{
	DECLARE_RENDER_PASS(SSDOSamplesGenerationRenderPass)

protected:
	SSDOSamplesVolume* _ssdoSamplesVolume;

public:
	SSDOSamplesGenerationRenderPass ();
	virtual ~SSDOSamplesGenerationRenderPass ();

	virtual void Init (const RenderSettings& settings);
	RenderVolumeCollection* Execute (const RenderScene* renderScene, const Camera* camera,
		const RenderSettings& settings, RenderVolumeCollection* rvc);

	bool IsAvailable (const RenderScene* renderScene, const Camera* camera,
		const RenderSettings& settings, const RenderVolumeCollection* rvc) const;

	void Clear ();
protected:
	void UpdateSamplesVolume (const RenderSettings& settings);

	void InitSamplesVolume (const RenderSettings& settings);
};

#endif
