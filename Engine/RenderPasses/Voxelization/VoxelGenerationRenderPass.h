#ifndef VOXELGENERATIONRENDERPASS_H
#define VOXELGENERATIONRENDERPASS_H

#include "RenderPasses/Container/ContainerRenderSubPassI.h"

#include "VoxelVolume.h"

class VoxelGenerationRenderPass : public ContainerRenderSubPassI
{
	DECLARE_RENDER_PASS(VoxelGenerationRenderPass)

protected:
	VoxelVolume* _voxelVolume;

public:
	VoxelGenerationRenderPass ();

	void Init (const RenderSettings& settings);
	RenderVolumeCollection* Execute (const RenderScene* renderScene, const Camera* camera,
		const RenderSettings& settings, RenderVolumeCollection* rvc);

	bool IsAvailable (const RenderScene* renderScene, const Camera* camera,
		const RenderSettings& settings, const RenderVolumeCollection* rvc) const;

	void Clear ();
protected:
	void UpdateVoxelVolumeBoundingBox (const RenderScene*);

	void InitVoxelVolume (const RenderSettings& settings);
	void UpdateVoxelVolume (const RenderSettings& settings);
};

#endif