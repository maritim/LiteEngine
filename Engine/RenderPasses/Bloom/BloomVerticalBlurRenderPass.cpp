#include "BloomVerticalBlurRenderPass.h"

bool BloomVerticalBlurRenderPass::IsAvailable (const RenderScene* renderScene, const Camera* camera,
	const RenderSettings& settings, const RenderVolumeCollection* rvc) const
{
	/*
	 * Check if bloom is enabled
	*/

	return settings.bloom_enabled;
}

glm::ivec2 BloomVerticalBlurRenderPass::GetPostProcessVolumeResolution (const RenderSettings& settings) const
{
	return glm::ivec2 (glm::vec2 (settings.resolution.width, settings.resolution.height) * settings.bloom_scale);
}
