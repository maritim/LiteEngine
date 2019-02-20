#ifndef BLOOMHORIZONTALBLURCONTAINERRENDERSUBPASS_H
#define BLOOMHORIZONTALBLURCONTAINERRENDERSUBPASS_H

#include "RenderPasses/Blur/HorizontalGaussianBlurContainerRenderSubPass.h"
#include "Core/Observer/ObserverI.h"

#include "Systems/Settings/SettingsObserverArgs.h"

class BloomHorizontalBlurContainerRenderSubPass : public HorizontalGaussianBlurContainerRenderSubPass, public ObserverI<SettingsObserverArgs>
{
protected:
	bool _enabled;
	glm::ivec2 _resolution;

public:
	BloomHorizontalBlurContainerRenderSubPass ();
	~BloomHorizontalBlurContainerRenderSubPass ();

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