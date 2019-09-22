#ifndef AMBIENTLIGHTRENDERPASS_H
#define AMBIENTLIGHTRENDERPASS_H

#include "RenderPasses/Container/ContainerRenderSubPassI.h"

#include "AmbientLightVolume.h"

class ENGINE_API AmbientLightRenderPass : public ContainerRenderSubPassI
{
	DECLARE_RENDER_PASS(AmbientLightRenderPass)

protected:
	AmbientLightVolume* _ambientLightVolume;

public:
	AmbientLightRenderPass ();
	~AmbientLightRenderPass ();

	virtual void Init (const RenderSettings& settings);
	virtual RenderVolumeCollection* Execute (const RenderScene* renderScene, const Camera* camera,
		const RenderSettings& settings, RenderVolumeCollection* rvc);
	virtual bool IsAvailable (const RenderScene* renderScene, const Camera* camera,
		const RenderSettings& settings, const RenderVolumeCollection* rvc) const;

	void Clear ();
};

#endif
