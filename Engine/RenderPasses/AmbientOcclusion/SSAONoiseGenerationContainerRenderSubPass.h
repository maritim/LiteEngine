#ifndef SSAONOISEGENERATIONCONTAINERRENDERSUBPASS_H
#define SSAONOISESGENERATIONCONTAINERRENDERSUBPASS_H

#include "RenderPasses/Container/ContainerRenderSubPassI.h"
#include "Core/Observer/ObserverI.h"

#include "SSAONoiseMapVolume.h"

#include "Systems/Settings/SettingsObserverArgs.h"

#define SSAO_NOISE_NOT_INIT 352

class SSAONoiseGenerationContainerRenderSubPass : public ContainerRenderSubPassI, public ObserverI<SettingsObserverArgs>
{
protected:
	bool _enabled;
	glm::vec2 _ssaoNoiseMapResolution;
	SSAONoiseMapVolume* _ssaoNoiseMapVolume;

public:
	SSAONoiseGenerationContainerRenderSubPass ();
	~SSAONoiseGenerationContainerRenderSubPass ();

	void Init ();
	RenderVolumeCollection* Execute (const Scene* scene, const Camera* camera, RenderVolumeCollection* rvc);

	bool IsAvailable (const Scene* scene, const Camera* camera, const RenderVolumeCollection* rvc) const;

	void Notify (Object* sender, const SettingsObserverArgs& args);
protected:
	void InitSettings ();
	void ClearSettings ();

	void InitNoiseMapVolume ();
};

#endif
