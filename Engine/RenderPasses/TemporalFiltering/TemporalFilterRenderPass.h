#ifndef TEMPORALFILTERRENDERPASS_H
#define TEMPORALFILTERRENDERPASS_H

#include "RenderPasses/PostProcess/PostProcessRenderPass.h"

class ENGINE_API TemporalFilterRenderPass : public PostProcessRenderPass
{
protected:
	FramebufferRenderVolume* _postProcessMapVolume2;
	glm::mat4 _lastViewProjectionMatrix;
	std::size_t _frameCount;

public:
	TemporalFilterRenderPass ();

	void Init (const RenderSettings& settings);
	RenderVolumeCollection* Execute (const RenderScene* renderScene, const Camera* camera,
		const RenderSettings& settings, RenderVolumeCollection* rvc);

	void Clear ();
protected:
	void StartPostProcessPass ();

	std::string GetPostProcessVolumeName () const;
	glm::ivec2 GetPostProcessVolumeResolution (const RenderSettings& settings) const;
	FramebufferRenderVolume* CreatePostProcessVolume (const RenderSettings& settings) const;

	std::vector<PipelineAttribute> GetCustomAttributes (const Camera* camera,
		const RenderSettings& settings, RenderVolumeCollection* rvc);

	void UpdateLastPostProcessMapVolume (const RenderSettings& settings);

	FramebufferRenderVolume* GetCurrentPostProcessMapVolume () const;
	FramebufferRenderVolume* GetLastPostProcessMapVolume () const;
};

#endif