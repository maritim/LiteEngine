#ifndef VOXELIZATIONDIRECTIONALSHADOWMAPRENDERPASS_H
#define VOXELIZATIONDIRECTIONALSHADOWMAPRENDERPASS_H

#include "Renderer/RenderPassI.h"

#include "VoxelShadowMapVolume.h"

class VoxelDirectionalShadowMapRenderPass : public RenderPassI
{
protected:
	std::string _staticShaderName;
	std::string _animationShaderName;
	VoxelShadowMapVolume* _voxelShadowMapVolume;

	bool _firstTime;

public:
	VoxelDirectionalShadowMapRenderPass ();
	virtual ~VoxelDirectionalShadowMapRenderPass ();

	void Init (const RenderSettings& settings);
	RenderVolumeCollection* Execute (const RenderScene* renderScene, const Camera* camera,
		const RenderSettings& settings, RenderVolumeCollection* rvc);

	void Clear ();
protected:
	void StartShadowMapPass ();
	void ShadowMapGeometryPass (const RenderScene* renderScene, Camera* lightCamera);
	void EndShadowMapPass ();

	Camera* GetLightCamera (const RenderScene* renderScene, const Camera* camera);

	void LockShader (int sceneLayers);
};

#endif