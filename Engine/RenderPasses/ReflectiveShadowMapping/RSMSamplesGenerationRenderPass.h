#ifndef RSMSAMPLESGENERATIONRENDERPASS_H
#define RSMSAMPLESGENERATIONRENDERPASS_H

#include "RenderPasses/VolumetricLightRenderPassI.h"

#include "RSMSamplesVolume.h"

class RSMSamplesGenerationRenderPass : public VolumetricLightRenderPassI
{
	DECLARE_RENDER_PASS(RSMSamplesGenerationRenderPass)

protected:
	RSMSamplesVolume* _rsmSamplesVolume;

public:
	RSMSamplesGenerationRenderPass ();

	virtual void Init(const RenderSettings& settings);
	virtual RenderVolumeCollection* Execute(const RenderScene* renderScene, const Camera* camera,
		const RenderSettings& settings, RenderVolumeCollection* rvc);

	virtual bool IsAvailable (const RenderLightObject*) const;

	void Clear ();
protected:
	virtual void UpdateRSMSamplesVolume (const RenderSettings& settings);
};

#endif
