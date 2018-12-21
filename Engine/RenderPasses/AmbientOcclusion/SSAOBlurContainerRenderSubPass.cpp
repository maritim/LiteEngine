#include "SSAOBlurContainerRenderSubPass.h"

#include "SSAOMapVolume.h"

bool SSAOBlurContainerRenderSubPass::IsAvailable (const Scene* scene, const Camera* camera, const RenderVolumeCollection* rvc) const
{
	/*
	 * TODO: Check if screen space ambient occlusion blur is enabled
	*/

	return true;
}

std::string SSAOBlurContainerRenderSubPass::GetPostProcessFragmentShaderPath () const
{
	return "Assets/Shaders/AmbientOcclusion/screenSpaceAmbientOcclusionBlurFragment.glsl";
}

std::string SSAOBlurContainerRenderSubPass::GetPostProcessVolumeName () const
{
	return "SSAOMapVolume";
}

PostProcessMapVolume* SSAOBlurContainerRenderSubPass::CreatePostProcessVolume () const
{
	SSAOMapVolume* ssaoBlurMapVolume = new SSAOMapVolume ();

	return ssaoBlurMapVolume;
}
