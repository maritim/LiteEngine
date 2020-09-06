#ifndef SSDOTEMPORALFILTERRENDERPASS_H
#define SSDOTEMPORALFILTERRENDERPASS_H

#include "RenderPasses/TemporalFiltering/TemporalFilterRenderPass.h"

class ENGINE_API SSDOTemporalFilterRenderPass : public TemporalFilterRenderPass
{
	DECLARE_RENDER_PASS(SSDOTemporalFilterRenderPass)

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