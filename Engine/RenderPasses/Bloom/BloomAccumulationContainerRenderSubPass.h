#ifndef BLOOMACCUMULATIONCONTAINERRENDERSUBPASS_H
#define BLOOMACCUMULATIONCONTAINERRENDERSUBPASS_H

#include "RenderPasses/PostProcess/PostProcessContainerRenderSubPass.h"
#include "Core/Observer/ObserverI.h"

#include "Systems/Settings/SettingsObserverArgs.h"

class BloomAccumulationContainerRenderSubPass : public PostProcessContainerRenderSubPass, public ObserverI<SettingsObserverArgs>
{
protected:
	bool _enabled;
	float _intensity;
	glm::ivec2 _resolution;

public:
	BloomAccumulationContainerRenderSubPass ();
	~BloomAccumulationContainerRenderSubPass ();

	void Init ();

	bool IsAvailable (const Scene* scene, const Camera* camera, const RenderVolumeCollection* rvc) const;

	void Notify (Object* sender, const SettingsObserverArgs& args);

	void Clear ();
protected:
	std::string GetPostProcessFragmentShaderPath () const;
	std::string GetPostProcessVolumeName () const;	
	glm::ivec2 GetPostProcessVolumeResolution () const;
	PostProcessMapVolume* CreatePostProcessVolume () const;

	std::vector<PipelineAttribute> GetCustomAttributes (RenderVolumeCollection* rvc);

	void InitSettings ();
	void ClearSettings ();
};

#endif