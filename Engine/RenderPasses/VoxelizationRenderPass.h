#ifndef VOXELIZATIONRENDERPASS_H
#define VOXELIZATIONRENDERPASS_H

#include "Renderer/RenderPassI.h"
#include "Core/Observer/ObserverI.h"

#include <string>

#include "VoxelVolume.h"

#include "Systems/Settings/SettingsObserverArgs.h"

class VoxelizationRenderPass : public RenderPassI, public ObserverI<SettingsObserverArgs>
{
protected:
	std::size_t _voxelVolumeSize;
	bool _continuousVoxelization;
	std::string _staticShaderName;
	std::string _animationShaderName;
	VoxelVolume* _voxelVolume;

	bool _firstTime;

public:
	VoxelizationRenderPass ();
	~VoxelizationRenderPass ();

	void Init ();
	RenderVolumeCollection* Execute (const Scene* scene, const Camera* camera, RenderVolumeCollection* rvc);

	void Notify (Object* sender, const SettingsObserverArgs& args);

	void Clear ();
protected:
	void StartVoxelization ();
	void GeometryVoxelizationPass (const Scene* scene);
	void EndVoxelization ();

	void LockShader (int sceneLayers);

	void UpdateVoxelVolumeBoundingBox (const Scene*);

	void InitSettings ();
	void ClearSettings ();

	void InitVoxelVolume ();
};

#endif