#include "GammaCorrectionContainerRenderSubPass.h"

bool GammaCorrectionContainerRenderSubPass::IsAvailable (const Scene* scene, const Camera* camera, const RenderVolumeCollection* rvc) const
{
	/*
	 * TODO: Check if gamma correction is enabled
	*/

	return true;
}

std::string GammaCorrectionContainerRenderSubPass::GetPostProcessFragmentShaderPath () const
{
	return "Assets/Shaders/GammaCorrection/gammaCorrectionFragment.glsl";
}

std::string GammaCorrectionContainerRenderSubPass::GetPostProcessVolumeName () const
{
	return "PostProcessMapVolume";
}

PostProcessMapVolume* GammaCorrectionContainerRenderSubPass::CreatePostProcessVolume () const
{
	PostProcessMapVolume* volume = new PostProcessMapVolume ();

	return volume;
}
