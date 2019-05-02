#ifndef DIRECTIONALLIGHTSHADOWMAPCONTAINERRENDERSUBPASS_H
#define DIRECTIONALLIGHTSHADOWMAPCONTAINERRENDERSUBPASS_H

#include "RenderPasses/VolumetricLightContainerRenderSubPassI.h"
#include "Core/Observer/ObserverI.h"

#include "RenderPasses/ShadowMap/CascadedShadowMapDirectionalLightVolume.h"

#include "Systems/Camera/Camera.h"
#include "Cameras/OrthographicCamera.h"
#include "Lighting/VolumetricLight.h"

#include "Systems/Settings/SettingsObserverArgs.h"

class DirectionalLightShadowMapContainerRenderSubPass : public VolumetricLightContainerRenderSubPassI, public ObserverI<SettingsObserverArgs>
{
protected:
	std::string _staticShaderName;
	std::string _animationShaderName;
	CascadedShadowMapDirectionalLightVolume* _volume;

	std::size_t _cascades;
	glm::ivec2 _resolution;

public:
	DirectionalLightShadowMapContainerRenderSubPass ();
	~DirectionalLightShadowMapContainerRenderSubPass ();

	virtual void Init (const RenderSettings& settings);
	virtual RenderVolumeCollection* Execute (const Scene* scene, const Camera* camera,
		const RenderSettings& settings, RenderVolumeCollection* rvc);

	void Notify (Object* sender, const SettingsObserverArgs& args);

	void Clear ();
protected:
	bool IsAvailable (const VolumetricLight*) const;

	void ShadowMapPass (const Scene* scene, const Camera* camera, VolumetricLight* volumetricLight);
	void EndShadowMapPass ();

	void UpdateCascadeLevelsLimits (const Camera* camera);
	void SendLightCamera (Camera* lightCamera);
	void UpdateLightCameras (const Camera* viewCamera, VolumetricLight* volumetricLight);
	void RenderScene (const Scene* scene, OrthographicCamera* lightCamera);
	void LockShader (int sceneLayers);

	virtual std::vector<PipelineAttribute> GetCustomAttributes () const;

	virtual void InitSettings ();
	virtual void ClearSettings ();

	virtual void InitShadowMapVolume ();
};

#endif