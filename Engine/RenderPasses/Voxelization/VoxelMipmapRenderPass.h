#ifndef VOXELMIPMAPRENDERPASS_H
#define VOXELMIPMAPRENDERPASS_H

#include "Renderer/RenderPassI.h"

#define MIPMAP_LEVELS 6

class VoxelMipmapRenderPass : public RenderPassI
{
protected:
	bool _firstTime;

public:
	VoxelMipmapRenderPass ();

	virtual void Init (const RenderSettings& settings);
	virtual RenderVolumeCollection* Execute (const RenderScene* renderScene, const Camera* camera,
		const RenderSettings& settings, RenderVolumeCollection* rvc);

	void Clear ();
protected:
	void StartVoxelMipmaping ();
	void GenerateMipmaps (const RenderSettings&, RenderVolumeCollection*);
	void EndVoxelMipmaping ();
};

#endif