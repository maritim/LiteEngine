#ifndef SPOTLIGHTSHADOWMAPRENDERPASS_H
#define SPOTLIGHTSHADOWMAPRENDERPASS_H

#include "RenderPasses/VolumetricLightRenderPassI.h"

#include "RenderPasses/ShadowMap/PerspectiveShadowMapVolume.h"

class ENGINE_API SpotLightShadowMapRenderPass : public VolumetricLightRenderPassI
{
protected:
	PerspectiveShadowMapVolume* _volume;

public:
	SpotLightShadowMapRenderPass ();

	virtual RenderVolumeCollection* Execute (const RenderScene* renderScene, const Camera* camera,
		const RenderSettings& settings, RenderVolumeCollection* rvc);

	void Clear ();
protected:
	bool IsAvailable (const RenderLightObject*) const;

	void ShadowMapPass (const RenderScene* renderScene, const Camera* camera);
	void EndShadowMapPass ();

	virtual void LockShader (int sceneLayers) = 0;

	virtual std::vector<PipelineAttribute> GetCustomAttributes () const;

	void UpdateLightCamera (const RenderLightObject* renderLightObject);
	void UpdateShadowMapVolume (const RenderLightObject* renderLightObject);
	void InitShadowMapVolume (const RenderLightObject* renderLightObject);
};

#endif