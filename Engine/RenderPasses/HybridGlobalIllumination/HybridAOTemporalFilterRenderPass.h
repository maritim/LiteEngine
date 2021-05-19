#ifndef HYBRIDAOTEMPORALFILTERRENDERPASS_H
#define HYBRIDAOTEMPORALFILTERRENDERPASS_H

#include "RenderPasses/TemporalFiltering/TemporalFilterRenderPass.h"

class ENGINE_API HybridAOTemporalFilterRenderPass : public TemporalFilterRenderPass
{
	DECLARE_RENDER_PASS(HybridAOTemporalFilterRenderPass)

public:
	bool IsAvailable (const RenderScene* renderScene, const Camera* camera,
		const RenderSettings& settings, const RenderVolumeCollection* rvc) const;
protected:
	std::string GetPostProcessFragmentShaderPath () const;
	std::string GetPostProcessVolumeName () const;
	glm::ivec2 GetPostProcessVolumeResolution (const RenderSettings& settings) const;
	FramebufferRenderVolume* CreatePostProcessVolume (const RenderSettings& settings) const;
};

#endif