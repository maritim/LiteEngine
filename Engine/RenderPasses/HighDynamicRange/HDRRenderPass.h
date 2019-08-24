#ifndef HDRRENDERPASS_H
#define HDRRENDERPASS_H

#include "RenderPasses/PostProcess/PostProcessRenderPass.h"

class ENGINE_API HDRRenderPass : public PostProcessRenderPass
{
	DECLARE_RENDER_PASS(HDRRenderPass)

public:
	bool IsAvailable (const RenderScene* renderScene, const Camera* camera,
		const RenderSettings& settings, const RenderVolumeCollection* rvc) const;
protected:
	std::string GetPostProcessFragmentShaderPath () const;
	std::string GetPostProcessVolumeName () const;	
	glm::ivec2 GetPostProcessVolumeResolution (const RenderSettings& settings) const;
	PostProcessMapVolume* CreatePostProcessVolume () const;

	std::vector<PipelineAttribute> GetCustomAttributes (const RenderSettings& settings, RenderVolumeCollection* rvc);
};

#endif