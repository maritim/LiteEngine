#ifndef REFLECTIVESHADOWMAPDIRECTIONALLIGHTACCUMULATIONCONTAINERRENDERSUBPASS_H
#define REFLECTIVESHADOWMAPDIRECTIONALLIGHTACCUMULATIONCONTAINERRENDERSUBPASS_H

#include "RenderPasses/VolumetricLightContainerRenderSubPassI.h"
#include "Core/Observer/ObserverI.h"

#include <string>

#include "ReflectiveShadowMapVolume.h"

#include "Systems/Settings/SettingsObserverArgs.h"

class ReflectiveShadowMapDirectionalLightAccumulationContainerRenderSubPass : public VolumetricLightContainerRenderSubPassI, public ObserverI<SettingsObserverArgs>
{
protected:
	glm::vec2 _rsmResolution;
	std::string _staticShaderName;
	std::string _animationShaderName;
	ReflectiveShadowMapVolume* _reflectiveShadowMapVolume;

public:
	ReflectiveShadowMapDirectionalLightAccumulationContainerRenderSubPass ();
	virtual ~ReflectiveShadowMapDirectionalLightAccumulationContainerRenderSubPass ();

	void Init (const RenderSettings& settings);
	RenderVolumeCollection* Execute (const Scene* scene, const Camera* camera,
		const RenderSettings& settings, RenderVolumeCollection* rvc);

	bool IsAvailable (const VolumetricLight*) const;

	void Notify (Object* sender, const SettingsObserverArgs& args);

	void Clear ();
protected:
	void StartShadowMapPass ();
	void ShadowMapGeometryPass (const Scene* scene, const Camera* lightCamera);
	void EndShadowMapPass ();

	void LockShader (int sceneLayers);

	Camera* GetLightCamera (const Scene* scene, const Camera* camera);

	void InitSettings ();
	void ClearSettings ();

	void InitRSMVolume ();
};

#endif