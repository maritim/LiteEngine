#ifndef VOXELIZATIONRENDERPASS_H
#define VOXELIZATIONRENDERPASS_H

#include "Renderer/RenderPassI.h"

#include "VoxelVolume.h"

class VoxelizationRenderPass : public RenderPassI
{
protected:
	VoxelVolume* _voxelVolume;

public:
	VoxelizationRenderPass ();
	~VoxelizationRenderPass ();

	void Init ();
	RenderVolumeCollection* Execute (const Scene* scene, const Camera* camera, RenderVolumeCollection* rvc);
protected:
	void StartVoxelization ();
	void GeometryVoxelizationPass (const Scene* scene);
	void EndVoxelization ();

	void UpdateVoxelVolumeBoundingBox (const Scene*);
};

#endif