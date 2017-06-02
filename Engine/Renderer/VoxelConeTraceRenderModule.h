#ifndef VOXELCONETRACINGREDRENDERMODULE_H
#define VOXELCONETRACINGREDRENDERMODULE_H

#include "DeferredRenderModule.h"
#include "VoxelizationRenderModule.h"

class VoxelConeTraceRenderModule : 
	public DeferredRenderModule, 
	public VoxelizationRenderModule
{
public:
	VoxelConeTraceRenderModule ();
	virtual ~VoxelConeTraceRenderModule ();

	virtual void RenderScene (Scene*, Camera*);
protected:
	void DeferredPass (Scene* scene, Camera* camera);

	void LightPass (Scene* scene, Camera* camera);

	void DirectionalLightPass (Scene* scene, Camera* camera);

	void StartLightPass ();
};

#endif