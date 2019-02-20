#ifndef BRIGHTEXTRACTIONCONTAINERRENDERSUBPASS_H
#define BRIGHTEXTRACTIONCONTAINERRENDERSUBPASS_H

#include "RenderPasses/PostProcess/PostProcessContainerRenderSubPass.h"
#include "Core/Observer/ObserverI.h"

#include "Systems/Settings/SettingsObserverArgs.h"

class BrightExtractionContainerRenderSubPass : public PostProcessContainerRenderSubPass, public ObserverI<SettingsObserverArgs>
{
protected:
	bool _enabled;
	float _threshold;
	glm::ivec2 _resolution;

public:
	BrightExtractionContainerRenderSubPass ();
	~BrightExtractionContainerRenderSubPass ();

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