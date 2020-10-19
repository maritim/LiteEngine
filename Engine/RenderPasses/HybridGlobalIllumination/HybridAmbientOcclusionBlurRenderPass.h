#ifndef HYBRIDAMBIENTOCCLUSIONBLURRENDERPASS_H
#define HYBRIDAMBIENTOCCLUSIONBLURRENDERPASS_H

#include "RenderPasses/ScreenSpaceAmbientOcclusion/SSAOBlurRenderPass.h"

class ENGINE_API HybridAmbientOcclusionBlurRenderPass : public SSAOBlurRenderPass
{
	DECLARE_RENDER_PASS(HybridAmbientOcclusionBlurRenderPass)

public:
	bool IsAvailable (const RenderScene* renderScene, const Camera* camera,
		const RenderSettings& settings, const RenderVolumeCollection* rvc) const;
protected:
	std::string GetPostProcessVolumeName () const;
	glm::ivec2 GetPostProcessVolumeResolution (const RenderSettings& settings) const;
	FramebufferRenderVolume* CreatePostProcessVolume (const RenderSettings& settings) const;

	std::vector<PipelineAttribute> GetCustomAttributes (const Camera* camera,
		const RenderSettings& settings, RenderVolumeCollection* rvc);
};

#endif