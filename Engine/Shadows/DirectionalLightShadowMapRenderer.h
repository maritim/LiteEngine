#ifndef DIRECTIONALLIGHTSHADOWMAPRENDERER_H
#define DIRECTIONALLIGHTSHADOWMAPRENDERER_H

#include "LightShadowMapRenderer.h"

#include "Cameras/OrthographicCamera.h"

class DirectionalLightShadowMapRenderer : public LightShadowMapRenderer
{
protected:
	OrthographicCamera* _lightCamera;

public:
	DirectionalLightShadowMapRenderer (Light* light);
protected:
	void ShadowMapRender (Scene* scene, Camera* camera);

	std::vector<PipelineAttribute> GetCustomAttributes ();

	void SendLightCamera(Camera* camera);
	void UpdateLightCamera(Camera* camera);
};

#endif