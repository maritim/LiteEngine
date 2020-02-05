#ifndef VOXELMIPMAPRENDERPASS_H
#define VOXELMIPMAPRENDERPASS_H

#include "Renderer/RenderPassI.h"

#include "Core/Resources/Resource.h"
#include "Renderer/RenderViews/ShaderView.h"

class VoxelMipmapRenderPass : public RenderPassI
{
	DECLARE_RENDER_PASS(VoxelMipmapRenderPass)

protected:
	Resource<ShaderView> _shaderView;
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