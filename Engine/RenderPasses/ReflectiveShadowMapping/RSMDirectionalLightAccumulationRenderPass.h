#ifndef RSMDIRECTIONALLIGHTACCUMULATIONRENDERPASS_H
#define RSMDIRECTIONALLIGHTACCUMULATIONRENDERPASS_H

#include "RenderPasses/VolumetricLightRenderPassI.h"

#include <string>

#include "RSMVolume.h"

class RSMDirectionalLightAccumulationRenderPass : public VolumetricLightRenderPassI
{
	DECLARE_RENDER_PASS(RSMDirectionalLightAccumulationRenderPass)

protected:
	std::string _staticShaderName;
	std::string _animationShaderName;
	RSMVolume* _reflectiveShadowMapVolume;

public:
	RSMDirectionalLightAccumulationRenderPass ();
	virtual ~RSMDirectionalLightAccumulationRenderPass ();

	void Init (const RenderSettings& settings);
	RenderVolumeCollection* Execute (const RenderScene* renderScene, const Camera* camera,
		const RenderSettings& settings, RenderVolumeCollection* rvc);

	bool IsAvailable (const RenderLightObject*) const;

	void Clear ();
protected:
	void StartShadowMapPass ();
	void ShadowMapGeometryPass (const RenderScene* renderScene, const Camera* lightCamera,
		const RenderSettings& settings);
	void EndShadowMapPass ();

	void LockShader (int sceneLayers);

	Camera* GetLightCamera (const RenderScene* renderScene, const Camera* camera);

	void InitRSMVolume (const RenderSettings& settings);
	void UpdateRSMVolume (const RenderSettings& settings);
};

#endif