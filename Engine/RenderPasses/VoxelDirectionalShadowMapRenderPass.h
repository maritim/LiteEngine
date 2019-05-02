#ifndef VOXELIZATIONDIRECTIONALSHADOWMAPRENDERPASS_H
#define VOXELIZATIONDIRECTIONALSHADOWMAPRENDERPASS_H

#include "Renderer/RenderPassI.h"
#include "Core/Observer/ObserverI.h"

#include "VoxelShadowMapVolume.h"

#include "Systems/Settings/SettingsObserverArgs.h"

class VoxelDirectionalShadowMapRenderPass : public RenderPassI, public ObserverI<SettingsObserverArgs>
{
protected:
	bool _continuousVoxelization;
	std::string _staticShaderName;
	std::string _animationShaderName;
	VoxelShadowMapVolume* _voxelShadowMapVolume;

	bool _firstTime;

public:
	VoxelDirectionalShadowMapRenderPass ();
	virtual ~VoxelDirectionalShadowMapRenderPass ();

	void Init (const RenderSettings& settings);
	RenderVolumeCollection* Execute (const Scene* scene, const Camera* camera,
		const RenderSettings& settings, RenderVolumeCollection* rvc);

	void Notify (Object* sender, const SettingsObserverArgs& args);

	void Clear ();
protected:
	void StartShadowMapPass ();
	void ShadowMapGeometryPass (const Scene* scene, Camera* lightCamera);
	void EndShadowMapPass ();

	Camera* GetLightCamera (const Scene* scene, const Camera* camera);

	void LockShader (int sceneLayers);

	void InitSettings ();
	void ClearSettings ();
};

#endif