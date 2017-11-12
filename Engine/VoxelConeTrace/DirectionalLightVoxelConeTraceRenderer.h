#ifndef DIRECTIONALLIGHTVOXELCONETRACERENDERER_H
#define DIRECTIONALLIGHTVOXELCONETRACERENDERER_H

#include "Shadows/DirectionalLightShadowMapRenderer.h"

class DirectionalLightVoxelConeTraceRenderer : public DirectionalLightShadowMapRenderer
{
protected:
	RenderVolumeCollection* _rvc;

public:
	DirectionalLightVoxelConeTraceRenderer (Light* light);
	~DirectionalLightVoxelConeTraceRenderer ();

	void Draw (Scene* scene, Camera* camera, RenderVolumeCollection* rvc);
protected:
	std::vector<PipelineAttribute> GetCustomAttributes ();
};

#endif