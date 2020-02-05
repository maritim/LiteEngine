#ifndef DIRECTIONALLIGHTSHADOWMAPRENDERPASS_H
#define DIRECTIONALLIGHTSHADOWMAPRENDERPASS_H

#include "RenderPasses/VolumetricLightRenderPassI.h"

#include "Core/Resources/Resource.h"
#include "Renderer/RenderViews/ShaderView.h"

#include "RenderPasses/ShadowMap/CascadedShadowMapDirectionalLightVolume.h"

#include "Systems/Camera/Camera.h"
#include "Cameras/OrthographicCamera.h"

class ENGINE_API DirectionalLightShadowMapRenderPass : public VolumetricLightRenderPassI
{
	DECLARE_RENDER_PASS(DirectionalLightShadowMapRenderPass)

protected:
	Resource<ShaderView> _staticShaderView;
	Resource<ShaderView> _animationShaderView;
	CascadedShadowMapDirectionalLightVolume* _volume;

public:
	DirectionalLightShadowMapRenderPass ();
	~DirectionalLightShadowMapRenderPass ();

	virtual void Init (const RenderSettings& settings);
	virtual RenderVolumeCollection* Execute (const RenderScene* renderScene, const Camera* camera,
		const RenderSettings& settings, RenderVolumeCollection* rvc);

	void Clear ();
protected:
	bool IsAvailable (const RenderLightObject*) const;

	void ShadowMapPass (const RenderScene* renderScene, const Camera* camera, const RenderLightObject* renderLightObject);
	void EndShadowMapPass ();

	void UpdateCascadeLevelsLimits (const Camera* camera, const RenderLightObject* renderLightObject);
	void SendLightCamera (Camera* lightCamera);
	void UpdateLightCameras (const Camera* viewCamera, const RenderLightObject* renderLightObject);
	void Render (const RenderScene* renderScene, OrthographicCamera* lightCamera);
	void LockShader (int sceneLayers);

	virtual std::vector<PipelineAttribute> GetCustomAttributes () const;

	virtual void UpdateShadowMapVolume (const RenderLightObject* renderLightObject);
	virtual void InitShadowMapVolume (const RenderLightObject* renderLightObject);
};

#endif