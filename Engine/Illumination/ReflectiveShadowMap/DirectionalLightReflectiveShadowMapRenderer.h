#ifndef DIRECTIONALLIGHTREFLECTIVESHADOWMAPPINGRENDERER_H
#define DIRECTIONALLIGHTREFLECTIVESHADOWMAPPINGRENDERER_H

#include "Shadows/DirectionalLightShadowMapRenderer.h"

class DirectionalLightReflectiveShadowMapRenderer : public DirectionalLightShadowMapRenderer
{
protected:
	RenderVolumeCollection* _rvc;

public:
	DirectionalLightReflectiveShadowMapRenderer (Light* light);
	~DirectionalLightReflectiveShadowMapRenderer ();

	void Draw (Scene* scene, Camera* camera, RenderVolumeCollection* rvc);
protected:
	std::vector<PipelineAttribute> GetCustomAttributes ();
};

#endif