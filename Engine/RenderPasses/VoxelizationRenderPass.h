#ifndef VOXELIZATIONRENDERPASS_H
#define VOXELIZATIONRENDERPASS_H

#include "Renderer/RenderPassI.h"

#include <string>

#include "VoxelVolume.h"

class VoxelizationRenderPass : public RenderPassI
{
protected:
	std::string _staticShaderName;
	std::string _animationShaderName;
	VoxelVolume* _voxelVolume;

public:
	VoxelizationRenderPass ();
	~VoxelizationRenderPass ();

	void Init ();
	RenderVolumeCollection* Execute (const Scene* scene, const Camera* camera, RenderVolumeCollection* rvc);

	void Clear ();
protected:
	void StartVoxelization ();
	void GeometryVoxelizationPass (const Scene* scene);
	void EndVoxelization ();

	void LockShader (int sceneLayers);

	void UpdateVoxelVolumeBoundingBox (const Scene*);
};

#endif