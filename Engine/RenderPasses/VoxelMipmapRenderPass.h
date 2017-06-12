#ifndef VOXELMIPMAPRENDERPASS_H
#define VOXELMIPMAPRENDERPASS_H

#include "Renderer/RenderPassI.h"

#define MIPMAP_LEVELS 6

class VoxelMipmapRenderPass : public RenderPassI
{
public:
	virtual ~VoxelMipmapRenderPass ();

	virtual void Init ();
	virtual RenderVolumeCollection* Execute (Scene* scene, Camera* camera, RenderVolumeCollection* rvc);
protected:
	void StartVoxelMipmaping ();
	void GenerateMipmaps (RenderVolumeCollection*);
	void EndVoxelMipmaping ();
};

#endif