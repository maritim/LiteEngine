#ifndef RSMDIRECTIONALLIGHTACCUMULATIONCONTAINERRENDERSUBPASS_H
#define RSMDIRECTIONALLIGHTACCUMULATIONCONTAINERRENDERSUBPASS_H

#include "RenderPasses/VolumetricLightContainerRenderSubPassI.h"

#include <string>

#include "RSMVolume.h"

class RSMDirectionalLightAccumulationContainerRenderSubPass : public VolumetricLightContainerRenderSubPassI
{
protected:
	std::string _staticShaderName;
	std::string _animationShaderName;
	RSMVolume* _reflectiveShadowMapVolume;

public:
	RSMDirectionalLightAccumulationContainerRenderSubPass ();
	virtual ~RSMDirectionalLightAccumulationContainerRenderSubPass ();

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