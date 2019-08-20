#ifndef VOXELRAYTRACERENDERPASS_H
#define VOXELRAYTRACERENDERPASS_H

#include "Renderer/RenderPassI.h"

class VoxelRayTraceRenderPass : public RenderPassI
{
public:
	virtual ~VoxelRayTraceRenderPass ();

	virtual void Init (const RenderSettings& settings);
	virtual RenderVolumeCollection* Execute (const RenderScene* renderScene, const Camera* camera,
		const RenderSettings& settings, RenderVolumeCollection* rvc);

	void Clear ();
protected:
	void StartRayTrace ();
	void VoxelRenderingRayTracePass (const Camera*, const RenderSettings&, RenderVolumeCollection*);
};

#endif