#ifndef TEMPORALFILTERRENDERPASS_H
#define TEMPORALFILTERRENDERPASS_H

#include "RenderPasses/PostProcess/PostProcessRenderPass.h"

class ENGINE_API TemporalFilterRenderPass : public PostProcessRenderPass
{
protected:
	PostProcessMapVolume* _temporalFilterMapVolume;

public:
	TemporalFilterRenderPass ();
	~TemporalFilterRenderPass ();

	void Init (const RenderSettings& settings);
	RenderVolumeCollection* Execute (const RenderScene* renderScene, const Camera* camera,
		const RenderSettings& settings, RenderVolumeCollection* rvc);

	void Clear ();
protected:
	std::string GetPostProcessFragmentShaderPath () const;
	std::string GetPostProcessVolumeName () const;
	glm::ivec2 GetPostProcessVolumeResolution (const RenderSettings& settings) const;
	PostProcessMapVolume* CreatePostProcessVolume () const;

	std::vector<PipelineAttribute> GetCustomAttributes (const Camera* camera,
		const RenderSettings& settings, RenderVolumeCollection* rvc);

	void UpdateLastPostProcessMapVolume (const RenderSettings& settings);
	void InitLastPostProcessMapVolume (const RenderSettings& settings);
};

#endif