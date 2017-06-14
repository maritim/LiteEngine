#ifndef VOXELBORDERRENDERPASS_H
#define VOXELBORDERRENDERPASS_H

#include "Renderer/RenderPassI.h"

class VoxelBorderRenderPass : public RenderPassI
{
public:
	virtual ~VoxelBorderRenderPass ();

	virtual void Init ();
	virtual RenderVolumeCollection* Execute (Scene* scene, Camera* camera, RenderVolumeCollection* rvc);
protected:
	void StartVoxelBordering ();
	void BorderVoxelVolume (RenderVolumeCollection*);
	void EndVoxelBordering ();
};

#endif