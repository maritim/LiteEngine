#ifndef RSMSAMPLESGENERATIONRENDERPASS_H
#define RSMSAMPLESGENERATIONRENDERPASS_H

#include "RenderPasses/VolumetricLightRenderPassI.h"

#include "RSMSamplesVolume.h"

class RSMSamplesGenerationRenderPass : public VolumetricLightRenderPassI
{
	DECLARE_RENDER_PASS(RSMSamplesGenerationRenderPass)

protected:
	RSMSamplesVolume* _reflectiveShadowMapSamplesVolume;

public:
	RSMSamplesGenerationRenderPass ();
	~RSMSamplesGenerationRenderPass ();

	virtual void Init(const RenderSettings& settings);
	virtual RenderVolumeCollection* Execute(const RenderScene* renderScene, const Camera* camera,
		const RenderSettings& settings, RenderVolumeCollection* rvc);

	virtual bool IsAvailable (const RenderLightObject*) const;

	void Clear ();
protected:
	void InitRSMSamplesVolume (const RenderSettings& settings);
	void UpdateRSMSamplesVolume (const RenderSettings& settings);
};

#endif
