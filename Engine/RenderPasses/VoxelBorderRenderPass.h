#ifndef VOXELBORDERRENDERPASS_H
#define VOXELBORDERRENDERPASS_H

#include "Renderer/RenderPassI.h"
#include "Core/Observer/ObserverI.h"

#include "Systems/Settings/SettingsObserverArgs.h"

class VoxelBorderRenderPass : public RenderPassI, public ObserverI<SettingsObserverArgs>
{
protected:
	bool _enabled;
	std::size_t _voxelVolumeSize;
	bool _continuousVoxelization;

	bool _firstTime;

public:
	VoxelBorderRenderPass ();

	virtual void Init ();
	virtual RenderVolumeCollection* Execute (const Scene* scene, const Camera* camera, RenderVolumeCollection* rvc);

	void Notify (Object* sender, const SettingsObserverArgs& args);

	void Clear ();
protected:
	void StartVoxelBordering ();
	void BorderVoxelVolume (RenderVolumeCollection*);
	void EndVoxelBordering ();

	void InitSettings ();
	void ClearSettings ();
};

#endif