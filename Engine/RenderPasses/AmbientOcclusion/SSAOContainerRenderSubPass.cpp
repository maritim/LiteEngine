#include "SSAOContainerRenderSubPass.h"

#include "SSAOMapVolume.h"

bool SSAOContainerRenderSubPass::IsAvailable (const Scene* scene, const Camera* camera, const RenderVolumeCollection* rvc) const
{
	/*
	 * TODO: Check if screen space ambient occlusion is enabled
	*/

	return true;
}

std::string SSAOContainerRenderSubPass::GetPostProcessFragmentShaderPath () const
{
	return "Assets/Shaders/AmbientOcclusion/screenSpaceAmbientOcclusionFragment.glsl";
}

std::string SSAOContainerRenderSubPass::GetPostProcessVolumeName () const
{
	return "SSAOMapVolume";
}

PostProcessMapVolume* SSAOContainerRenderSubPass::CreatePostProcessVolume () const
{
	SSAOMapVolume* ssaoMapVolume = new SSAOMapVolume ();

	return ssaoMapVolume;
}
