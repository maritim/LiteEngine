#ifndef REFLECTIVESHADOWMAPSAMPLESGENERATIONCONTAINERRENDERSUBPASS_H
#define REFLECTIVESHADOWMAPSAMPLESGENERATIONCONTAINERRENDERSUBPASS_H

#include "RenderPasses/VolumetricLightContainerRenderSubPassI.h"
#include "Core/Observer/ObserverI.h"

#include "ReflectiveShadowMapSamplesVolume.h"

#include "Systems/Settings/SettingsObserverArgs.h"

class ReflectiveShadowMapSamplesGenerationContainerRenderSubPass : public VolumetricLightContainerRenderSubPassI, public ObserverI<SettingsObserverArgs>
{
protected:
	std::size_t _samplesSize;
	ReflectiveShadowMapSamplesVolume* _reflectiveShadowMapSamplesVolume;

public:
	ReflectiveShadowMapSamplesGenerationContainerRenderSubPass ();
	~ReflectiveShadowMapSamplesGenerationContainerRenderSubPass ();

	virtual void Init(const RenderSettings& settings);
	virtual RenderVolumeCollection* Execute(const Scene* scene, const Camera* camera,
		const RenderSettings& settings, RenderVolumeCollection* rvc);

	virtual bool IsAvailable (const VolumetricLight*) const;

	void Notify (Object* sender, const SettingsObserverArgs& args);

	void Clear ();
protected:
	void InitSettings ();
	void ClearSettings ();

	void InitRSMSamplesVolume ();
};

#endif
