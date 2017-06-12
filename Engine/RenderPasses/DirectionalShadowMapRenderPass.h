#ifndef DIRECTIONALSHADOWMAPRENDERPASS_H
#define DIRECTIONALSHADOWMAPRENDERPASS_H

#include "Renderer/RenderPassI.h"

#include "VoxelShadowMapVolume.h"

class DirectionalShadowMapRenderPass : public RenderPassI
{
protected:
	VoxelShadowMapVolume* _voxelShadowMapVolume;

public:
	DirectionalShadowMapRenderPass ();
	virtual ~DirectionalShadowMapRenderPass ();

	void Init ();
	RenderVolumeCollection* Execute (Scene* scene, Camera* camera, RenderVolumeCollection* rvc);
protected:
	void StartShadowMapPass ();
	void ShadowMapGeometryPass (Scene* scene, Camera* lightCamera);
	void EndShadowMapPass ();

	Camera* GetLightCamera (Scene* scene, Camera* camera);
};

#endif