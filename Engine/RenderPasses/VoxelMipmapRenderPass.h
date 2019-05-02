#ifndef VOXELMIPMAPRENDERPASS_H
#define VOXELMIPMAPRENDERPASS_H

#include "Renderer/RenderPassI.h"
#include "Core/Observer/ObserverI.h"

#include "Systems/Settings/SettingsObserverArgs.h"

#define MIPMAP_LEVELS 6

class VoxelMipmapRenderPass : public RenderPassI, public ObserverI<SettingsObserverArgs>
{
protected:
	std::size_t _voxelVolumeSize;
	bool _continuousVoxelization;

	bool _firstTime;

public:
	VoxelMipmapRenderPass ();

	virtual void Init (const RenderSettings& settings);
	virtual RenderVolumeCollection* Execute (const Scene* scene, const Camera* camera,
		const RenderSettings& settings, RenderVolumeCollection* rvc);

	void Notify (Object* sender, const SettingsObserverArgs& args);

	void Clear ();
protected:
	void StartVoxelMipmaping ();
	void GenerateMipmaps (RenderVolumeCollection*);
	void EndVoxelMipmaping ();

	void InitSettings ();
	void ClearSettings ();
};

#endif