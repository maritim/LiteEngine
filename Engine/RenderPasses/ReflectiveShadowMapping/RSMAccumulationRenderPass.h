#ifndef RSMACCUMULATIONRENDERPASS_H
#define RSMACCUMULATIONRENDERPASS_H

#include "RenderPasses/VolumetricLightRenderPassI.h"

#include "RSMVolume.h"

class RSMAccumulationRenderPass : public VolumetricLightRenderPassI
{
protected:
	RSMVolume* _rsmVolume;

public:
	RSMAccumulationRenderPass ();

	RenderVolumeCollection* Execute (const RenderScene* renderScene, const Camera* camera,
		const RenderSettings& settings, RenderVolumeCollection* rvc);

	bool IsAvailable (const RenderLightObject*) const;

	void Clear ();
protected:
	void StartShadowMapPass ();
	void ShadowMapGeometryPass (const RenderScene* renderScene, const Camera* lightCamera,
		const RenderSettings& settings, const RenderLightObject* renderLightObject);
	void EndShadowMapPass ();

	virtual void LockShader (int sceneLayers) = 0;
	virtual Camera* GetLightCamera (const RenderScene* renderScene, const RenderLightObject* renderLightObject) = 0;

	void InitRSMVolume (const RenderLightObject* renderLightObject);
	void UpdateRSMVolume (const RenderLightObject* renderLightObject);

	virtual std::vector<PipelineAttribute> GetCustomAttributes (const RenderLightObject* renderLightObject) const;
};

#endif