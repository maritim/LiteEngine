#ifndef VOXELRADIANCEINJECTIONRENDERPASS_H
#define VOXELRADIANCEINJECTIONRENDERPASS_H

#include "Renderer/RenderPassI.h"
#include "Core/Observer/ObserverI.h"

#include "Systems/Settings/SettingsObserverArgs.h"

class VoxelRadianceInjectionRenderPass : public RenderPassI, public ObserverI<SettingsObserverArgs>
{
protected:
	std::size_t _voxelVolumeSize;
	bool _continuousVoxelization;

	bool _firstTime;

public:
	VoxelRadianceInjectionRenderPass ();

	virtual void Init ();
	virtual RenderVolumeCollection* Execute (const Scene* scene, const Camera* camera, RenderVolumeCollection* rvc);

	void Notify (Object* sender, const SettingsObserverArgs& args);

	void Clear ();
protected:
	void StartRadianceInjectionPass ();
	void RadianceInjectPass (RenderVolumeCollection*);
	void EndRadianceInjectionPass ();

	void InitSettings ();
	void ClearSettings ();
};

#endif