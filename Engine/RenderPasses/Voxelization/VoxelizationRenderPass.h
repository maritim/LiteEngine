#ifndef VOXELIZATIONRENDERPASS_H
#define VOXELIZATIONRENDERPASS_H

#include "RenderPasses/Container/ContainerRenderSubPassI.h"

#include "Core/Resources/Resource.h"
#include "Renderer/RenderViews/ShaderView.h"

#include "VoxelVolume.h"

class VoxelizationRenderPass : public ContainerRenderSubPassI
{
	DECLARE_RENDER_PASS(VoxelizationRenderPass)

protected:
	Resource<ShaderView> _staticShaderView;
	Resource<ShaderView> _animationShaderView;

public:
	void Init (const RenderSettings& settings);
	RenderVolumeCollection* Execute (const RenderScene* renderScene, const Camera* camera,
		const RenderSettings& settings, RenderVolumeCollection* rvc);

	bool IsAvailable (const RenderScene* renderScene, const Camera* camera,
		const RenderSettings& settings, const RenderVolumeCollection* rvc) const;

	void Clear ();
protected:
	void StartVoxelization (VoxelVolume* voxelVolume);
	void GeometryVoxelizationPass (const RenderScene* renderScene, const RenderSettings& settings, VoxelVolume* voxelVolume);
	void EndVoxelization ();

	void LockShader (int sceneLayers);

	std::vector<PipelineAttribute> GetCustomAttributes (VoxelVolume* voxelVolume);
};

#endif