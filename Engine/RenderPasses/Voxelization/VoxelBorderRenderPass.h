#ifndef VOXELBORDERRENDERPASS_H
#define VOXELBORDERRENDERPASS_H

#include "RenderPasses/Container/ContainerRenderSubPassI.h"

#include "Core/Resources/Resource.h"
#include "Renderer/RenderViews/ShaderView.h"

class VoxelBorderRenderPass : public ContainerRenderSubPassI
{
	DECLARE_RENDER_PASS(VoxelBorderRenderPass)

protected:
	Resource<ShaderView> _shaderView;

public:
	virtual void Init (const RenderSettings& settings);
	virtual RenderVolumeCollection* Execute (const RenderScene* renderScene, const Camera* camera,
		const RenderSettings& settings, RenderVolumeCollection* rvc);

	void Clear ();

	bool IsAvailable (const RenderScene* renderScene, const Camera* camera,
		const RenderSettings& settings, const RenderVolumeCollection* rvc) const;
protected:
	void StartVoxelBordering ();
	void BorderVoxelVolume (const RenderSettings&, RenderVolumeCollection*);
	void EndVoxelBordering ();
};

#endif