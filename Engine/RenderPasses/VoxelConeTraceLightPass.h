#ifndef VOXELCONETRACELIGHTPASS_H
#define VOXELCONETRACELIGHTPASS_H

#include "Renderer/RenderPassI.h"

class VoxelConeTraceLightPass : public RenderPassI
{
public:
	virtual ~VoxelConeTraceLightPass ();

	virtual void Init ();
	virtual RenderVolumeCollection* Execute (Scene* scene, Camera* camera, RenderVolumeCollection* rvc);
protected:
	void LightPass (Scene* scene, Camera* camera, RenderVolumeCollection* rvc);
	void DirectionalLightPass (Scene* scene, Camera* camera, RenderVolumeCollection* rvc);
};

#endif