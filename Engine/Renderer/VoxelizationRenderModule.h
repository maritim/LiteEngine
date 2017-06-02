#ifndef VOXELIZATIONRENDERMODULE_H
#define VOXELIZATIONRENDERMODULE_H

#include "RenderModule.h"

#include "VoxelVolume.h"

class VoxelizationRenderModule : virtual public RenderModule
{
protected:
	VoxelVolume* _voxelVolume;

public:
	VoxelizationRenderModule ();
	virtual ~VoxelizationRenderModule ();

	virtual void RenderScene (Scene*, Camera*);
protected:
	void UpdateCamera (Camera*);

	void VoxelizePass (Scene*);
	void VoxelRayTracePass ();

	void StartVoxelization ();
	void GeometryVoxelizationPass (Scene* scene);
	void EndVoxelization ();

	void StartRayTrace ();
	void VoxelRenderingRayTracePass ();

	void UpdateVoxelVolumeBoundingBox (Scene*);
};

#endif