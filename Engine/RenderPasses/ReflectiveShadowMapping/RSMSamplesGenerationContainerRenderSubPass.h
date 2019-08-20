#ifndef RSMSAMPLESGENERATIONCONTAINERRENDERSUBPASS_H
#define RSMSAMPLESGENERATIONCONTAINERRENDERSUBPASS_H

#include "RenderPasses/VolumetricLightContainerRenderSubPassI.h"

#include "RSMSamplesVolume.h"

class RSMSamplesGenerationContainerRenderSubPass : public VolumetricLightContainerRenderSubPassI
{
protected:
	RSMSamplesVolume* _reflectiveShadowMapSamplesVolume;

public:
	RSMSamplesGenerationContainerRenderSubPass ();
	~RSMSamplesGenerationContainerRenderSubPass ();

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
