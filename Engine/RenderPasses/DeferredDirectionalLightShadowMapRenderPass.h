#ifndef DEFERREDDIRECTIONLIGHTSHADOWMAPRENDERPASS_H
#define DEFERREDDIRECTIONLIGHTSHADOWMAPRENDERPASS_H

#include "VolumetricLightContainerRenderSubPassI.h"

#include "Shadows/ShadowMapDirectionalLightVolume.h"

#include "Systems/Camera/Camera.h"
#include "Cameras/OrthographicCamera.h"
#include "Lighting/VolumetricLight.h"

//TODO: Change this from here
#define CASCADED_SHADOW_MAP_LEVELS 4

class DeferredDirectionalLightShadowMapRenderPass : public VolumetricLightContainerRenderSubPassI
{
protected:
	std::string _staticShaderName;
	std::string _animationShaderName;
	ShadowMapDirectionalLightVolume* _volume;

public:
	DeferredDirectionalLightShadowMapRenderPass ();
	~DeferredDirectionalLightShadowMapRenderPass ();

	virtual void Init ();
	virtual RenderVolumeCollection* Execute (const Scene* scene, const Camera* camera, RenderVolumeCollection* rvc);
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