#ifndef DIRECTIONALLIGHTVOXELCONETRACERENDERER_H
#define DIRECTIONALLIGHTVOXELCONETRACERENDERER_H

#include "Shadows/DirectionalLightShadowMapRenderer.h"

class DirectionalLightVoxelConeTraceRenderer : public DirectionalLightShadowMapRenderer
{
protected:
	VoxelVolume* _voxelVolume;

public:
	DirectionalLightVoxelConeTraceRenderer (Light* light);
	~DirectionalLightVoxelConeTraceRenderer ();

	void Draw (Scene* scene, Camera* camera, GBuffer* gBuffer, VoxelVolume* voxelVolume);
protected:
	std::vector<PipelineAttribute> GetCustomAttributes ();
};

#endif