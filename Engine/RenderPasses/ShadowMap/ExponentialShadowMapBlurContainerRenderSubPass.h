#ifndef EXPONENTIALSHADOWMAPBLURCONTAINERRENDERSUBPASS_H
#define EXPONENTIALSHADOWMAPBLURCONTAINERRENDERSUBPASS_H

#include "RenderPasses/VolumetricLightContainerRenderSubPassI.h"
#include "Core/Observer/ObserverI.h"

#include "ExponentialShadowMapBlurVolume.h"

#include "Systems/Settings/SettingsObserverArgs.h"

class ExponentialShadowMapBlurContainerRenderSubPass : public VolumetricLightContainerRenderSubPassI, public ObserverI<SettingsObserverArgs>
{
protected:
	std::string _shaders [2];
	ExponentialShadowMapBlurVolume** _framebuffers;

	glm::ivec2 _resolution;
	std::size_t _cascades;

public:
	ExponentialShadowMapBlurContainerRenderSubPass ();
	~ExponentialShadowMapBlurContainerRenderSubPass ();

	void Init ();
	RenderVolumeCollection* Execute (const RenderScene* renderScene, const Camera* camera, RenderVolumeCollection* rvc);

	void Notify (Object* sender, const SettingsObserverArgs& args);

	void Clear ();
protected:
	bool IsAvailable (const RenderLightObject*) const;

	void Blur (ExponentialShadowMapBlurVolume* fb1, ExponentialShadowMapBlurVolume* fb2);

	void InitSettings ();
	void ClearSettings ();

	void InitShadowMapBlurVolumes ();
};

#endif