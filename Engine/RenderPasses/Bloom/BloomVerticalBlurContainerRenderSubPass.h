#ifndef BLOOMVERTICALBLURCONTAINERRENDERSUBPASS_H
#define BLOOMVERTICALBLURCONTAINERRENDERSUBPASS_H

#include "RenderPasses/Blur/VerticalGaussianBlurContainerRenderSubPass.h"
#include "Core/Observer/ObserverI.h"

#include "Systems/Settings/SettingsObserverArgs.h"

class BloomVerticalBlurContainerRenderSubPass : public VerticalGaussianBlurContainerRenderSubPass, public ObserverI<SettingsObserverArgs>
{
protected:
	bool _enabled;
	glm::ivec2 _resolution;

public:
	BloomVerticalBlurContainerRenderSubPass ();
	~BloomVerticalBlurContainerRenderSubPass ();

	void Init ();

	bool IsAvailable (const Scene* scene, const Camera* camera, const RenderVolumeCollection* rvc) const;

	void Notify (Object* sender, const SettingsObserverArgs& args);

	void Clear ();
protected:
	glm::ivec2 GetPostProcessVolumeResolution () const;

	void InitSettings ();
	void ClearSettings ();
};

#endif