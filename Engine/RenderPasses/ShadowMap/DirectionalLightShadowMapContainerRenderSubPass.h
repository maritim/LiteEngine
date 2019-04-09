#ifndef DIRECTIONALLIGHTSHADOWMAPCONTAINERRENDERSUBPASS_H
#define DIRECTIONALLIGHTSHADOWMAPCONTAINERRENDERSUBPASS_H

#include "RenderPasses/VolumetricLightContainerRenderSubPassI.h"

#include "RenderPasses/ShadowMap/CascadedShadowMapDirectionalLightVolume.h"

#include "Systems/Camera/Camera.h"
#include "Cameras/OrthographicCamera.h"
#include "Lighting/VolumetricLight.h"

//TODO: Change this from here
#define CASCADED_SHADOW_MAP_LEVELS 4

class DirectionalLightShadowMapContainerRenderSubPass : public VolumetricLightContainerRenderSubPassI
{
protected:
	std::string _staticShaderName;
	std::string _animationShaderName;
	CascadedShadowMapDirectionalLightVolume* _volume;

public:
	DirectionalLightShadowMapContainerRenderSubPass ();
	~DirectionalLightShadowMapContainerRenderSubPass ();

	virtual void Init ();
	virtual RenderVolumeCollection* Execute (const Scene* scene, const Camera* camera, RenderVolumeCollection* rvc);

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
};

#endif