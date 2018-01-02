#ifndef DIRECTIONALSHADOWMAPRENDERPASS_H
#define DIRECTIONALSHADOWMAPRENDERPASS_H

#include "Renderer/RenderPassI.h"

#include "VoxelShadowMapVolume.h"

class DirectionalShadowMapRenderPass : public RenderPassI
{
protected:
	std::string _staticShaderName;
	std::string _animationShaderName;
	VoxelShadowMapVolume* _voxelShadowMapVolume;

public:
	DirectionalShadowMapRenderPass ();
	virtual ~DirectionalShadowMapRenderPass ();

	void Init ();
	RenderVolumeCollection* Execute (const Scene* scene, const Camera* camera, RenderVolumeCollection* rvc);
protected:
	void StartShadowMapPass ();
	void ShadowMapGeometryPass (const Scene* scene, Camera* lightCamera);
	void EndShadowMapPass ();

	Camera* GetLightCamera (const Scene* scene, const Camera* camera);

	void LockShader (int sceneLayers);
};

#endif