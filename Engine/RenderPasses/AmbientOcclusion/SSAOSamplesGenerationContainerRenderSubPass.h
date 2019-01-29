#ifndef SSAOSAMPLESGENERATIONCONTAINERRENDERSUBPASS_H
#define SSAOSAMPLESGENERATIONCONTAINERRENDERSUBPASS_H

#include "RenderPasses/Container/ContainerRenderSubPassI.h"
#include "Core/Observer/ObserverI.h"

#include "SSAOSamplesVolume.h"

#include "Systems/Settings/SettingsObserverArgs.h"

class SSAOSamplesGenerationContainerRenderSubPass : public ContainerRenderSubPassI, ObserverI<SettingsObserverArgs>
{
protected:
	bool _enabled;
	std::size_t _samplesSize;
	SSAOSamplesVolume* _ssaoSamplesVolume;

public:
	SSAOSamplesGenerationContainerRenderSubPass ();
	virtual ~SSAOSamplesGenerationContainerRenderSubPass ();

	virtual void Init ();
	RenderVolumeCollection* Execute (const Scene* scene, const Camera* camera, RenderVolumeCollection* rvc);

	bool IsAvailable (const Scene* scene, const Camera* camera, const RenderVolumeCollection* rvc) const;

	void Notify (Object* sender, const SettingsObserverArgs& args);

	void Clear ();
protected:
	void InitSettings ();
	void ClearSettings ();

	void InitSamplesVolume ();
};

#endif
