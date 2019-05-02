#include "BloomVerticalBlurContainerRenderSubPass.h"

bool BloomVerticalBlurContainerRenderSubPass::IsAvailable (const Scene* scene, const Camera* camera,
	const RenderSettings& settings, const RenderVolumeCollection* rvc) const
{
	/*
	 * Check if bloom is enabled
	*/

	return settings.bloom_enabled;
}

glm::ivec2 BloomVerticalBlurContainerRenderSubPass::GetPostProcessVolumeResolution (const RenderSettings& settings) const
{
	return glm::ivec2 (glm::vec2 (settings.framebuffer.width, settings.framebuffer.height) * settings.bloom_scale);
}
