#ifndef VOXELRAYTRACERENDERPASS_H
#define VOXELRAYTRACERENDERPASS_H

#include "Renderer/RenderPassI.h"

class VoxelRayTraceRenderPass : public RenderPassI
{
public:
	virtual ~VoxelRayTraceRenderPass ();

	virtual void Init ();
	virtual RenderVolumeCollection* Execute (Scene* scene, Camera* camera, RenderVolumeCollection* rvc);
protected:
	void StartRayTrace ();
	void VoxelRenderingRayTracePass (Camera*, RenderVolumeCollection*);
};

#endif