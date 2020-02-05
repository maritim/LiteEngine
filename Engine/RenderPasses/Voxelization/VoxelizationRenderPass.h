#ifndef VOXELIZATIONRENDERPASS_H
#define VOXELIZATIONRENDERPASS_H

#include "Renderer/RenderPassI.h"

#include "Core/Resources/Resource.h"
#include "Renderer/RenderViews/ShaderView.h"

#include "VoxelVolume.h"

class VoxelizationRenderPass : public RenderPassI
{
	DECLARE_RENDER_PASS(VoxelizationRenderPass)

protected:
	Resource<ShaderView> _staticShaderView;
	Resource<ShaderView> _animationShaderView;
	VoxelVolume* _voxelVolume;

	bool _firstTime;

public:
	VoxelizationRenderPass ();
	~VoxelizationRenderPass ();

	void Init (const RenderSettings& settings);
	RenderVolumeCollection* Execute (const RenderScene* renderScene, const Camera* camera,
		const RenderSettings& settings, RenderVolumeCollection* rvc);

	void Clear ();
protected:
	void StartVoxelization ();
	void GeometryVoxelizationPass (const RenderScene* renderScene, const RenderSettings& settings);
	void EndVoxelization ();

	void LockShader (int sceneLayers);

	void UpdateVoxelVolumeBoundingBox (const RenderScene*);

	void InitVoxelVolume (const RenderSettings& settings);
	void UpdateVoxelVolume (const RenderSettings& settings);
};

#endif