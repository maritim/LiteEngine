#ifndef SSAOTEMPORALFILTERRENDERPASS_H
#define SSAOTEMPORALFILTERRENDERPASS_H

#include "RenderPasses/TemporalFiltering/TemporalFilterRenderPass.h"

class ENGINE_API SSAOTemporalFilterRenderPass : public TemporalFilterRenderPass
{
	DECLARE_RENDER_PASS(SSAOTemporalFilterRenderPass)

public:
	bool IsAvailable (const RenderScene* renderScene, const Camera* camera,
		const RenderSettings& settings, const RenderVolumeCollection* rvc) const;
protected:
	std::string GetPostProcessFragmentShaderPath () const;
	std::string GetPostProcessVolumeName () const;
	glm::ivec2 GetPostProcessVolumeResolution (const RenderSettings& settings) const;
	FramebufferRenderVolume* CreatePostProcessVolume (const RenderSettings& settings) const;

	std::vector<PipelineAttribute> GetCustomAttributes (const Camera* camera,
		const RenderSettings& settings, RenderVolumeCollection* rvc);
};

#endif