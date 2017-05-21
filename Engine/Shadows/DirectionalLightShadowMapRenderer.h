#ifndef DIRECTIONALLIGHTSHADOWMAPRENDERER_H
#define DIRECTIONALLIGHTSHADOWMAPRENDERER_H

#include "LightShadowMapRenderer.h"

#include "Cameras/OrthographicCamera.h"

class DirectionalLightShadowMapRenderer : public LightShadowMapRenderer
{
protected:
	OrthographicCamera** _lightCameras;
	float* _shadowMapZEnd;

public:
	DirectionalLightShadowMapRenderer (Light* light);
protected:
	void ShadowMapRender (Scene* scene, Camera* camera);

	std::vector<PipelineAttribute> GetCustomAttributes ();

	void SendLightCamera (Camera* camera);
	void UpdateCascadeLevelsLimits (Camera* camera);
	void UpdateLightCameras (Camera* camera);

	void RenderScene (Scene* scene, OrthographicCamera* lightCamera);
};

#endif