#include "GammaCorrectionRenderPass.h"

bool GammaCorrectionRenderPass::IsAvailable (const RenderScene* renderScene, const Camera* camera,
	const RenderSettings& settings, const RenderVolumeCollection* rvc) const
{
	/*
	 * Check if gamma correction is enabled
	*/

	return settings.gamma_enabled;
}

std::string GammaCorrectionRenderPass::GetPostProcessFragmentShaderPath () const
{
	return "Assets/Shaders/GammaCorrection/gammaCorrectionFragment.glsl";
}

std::string GammaCorrectionRenderPass::GetPostProcessVolumeName () const
{
	return "PostProcessMapVolume";
}

glm::ivec2 GammaCorrectionRenderPass::GetPostProcessVolumeResolution (const RenderSettings& settings) const
{
	return glm::ivec2 (settings.framebuffer.width, settings.framebuffer.height);
}

PostProcessMapVolume* GammaCorrectionRenderPass::CreatePostProcessVolume () const
{
	PostProcessMapVolume* volume = new PostProcessMapVolume ();

	return volume;
}
