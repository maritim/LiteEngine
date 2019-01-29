#ifndef VOXELRAYTRACERENDERPASS_H
#define VOXELRAYTRACERENDERPASS_H

#include "Renderer/RenderPassI.h"

class VoxelRayTraceRenderPass : public RenderPassI
{
public:
	virtual ~VoxelRayTraceRenderPass ();

	virtual void Init ();
	virtual RenderVolumeCollection* Execute (const Scene* scene, const Camera* camera, RenderVolumeCollection* rvc);

	void Clear ();
protected:
	void StartRayTrace ();
	void VoxelRenderingRayTracePass (const Camera*, RenderVolumeCollection*);
};

#endif