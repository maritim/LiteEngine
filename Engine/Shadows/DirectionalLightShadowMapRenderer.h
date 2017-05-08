#ifndef DIRECTIONALLIGHTSHADOWMAPRENDERER_H
#define DIRECTIONALLIGHTSHADOWMAPRENDERER_H

#include "LightShadowMapRenderer.h"

#include "Cameras/OrthographicCamera.h"

class DirectionalLightShadowMapRenderer : public LightShadowMapRenderer
{
public:
	DirectionalLightShadowMapRenderer (Light* light);
protected:
	void ShadowMapRender (Scene* scene, Camera* camera);

	std::vector<PipelineAttribute> GetCustomAttributes ();

	void SendLightCamera(Camera* camera);
	OrthographicCamera* CreateLightCamera(Camera* camera);
};

#endif