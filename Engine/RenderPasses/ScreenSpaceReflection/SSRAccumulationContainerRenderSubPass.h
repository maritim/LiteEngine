#ifndef SSRACCUMULATIONCONTAINERRENDERSUBPASS_H
#define SSRACCUMULATIONCONTAINERRENDERSUBPASS_H

#include "RenderPasses/PostProcess/PostProcessContainerRenderSubPass.h"
#include "Core/Observer/ObserverI.h"

#include "Systems/Settings/SettingsObserverArgs.h"

class SSRAccumulationContainerRenderSubPass : public PostProcessContainerRenderSubPass, public ObserverI<SettingsObserverArgs>
{
protected:
	bool _enabled;

public:
	SSRAccumulationContainerRenderSubPass ();
	~SSRAccumulationContainerRenderSubPass ();

	void Init ();

	bool IsAvailable (const Scene* scene, const Camera* camera, const RenderVolumeCollection* rvc) const;

	void Notify (Object* sender, const SettingsObserverArgs& args);

	void Clear ();
protected:
	std::string GetPostProcessFragmentShaderPath () const;
	std::string GetPostProcessVolumeName () const;
	glm::ivec2 GetPostProcessVolumeResolution () const;
	PostProcessMapVolume* CreatePostProcessVolume () const;

	void InitSettings ();
	void ClearSettings ();
};

#endif