#include "HorizontalGaussianBlurContainerRenderSubPass.h"

#include "BlurMapVolume.h"

std::string HorizontalGaussianBlurContainerRenderSubPass::GetPostProcessFragmentShaderPath () const
{
	return "Assets/Shaders/Blur/horizontalGaussianBlurFragment.glsl";
}

std::string HorizontalGaussianBlurContainerRenderSubPass::GetPostProcessVolumeName () const
{
	return "BlurMapVolume";
}

PostProcessMapVolume* HorizontalGaussianBlurContainerRenderSubPass::CreatePostProcessVolume () const
{
	BlurMapVolume* blurMapVolume = new BlurMapVolume ();

	return blurMapVolume;
}

std::vector<PipelineAttribute> HorizontalGaussianBlurContainerRenderSubPass::GetCustomAttributes (RenderVolumeCollection* rvc)
{
	/*
	 * Attach post process volume attributes to pipeline
	*/

	std::vector<PipelineAttribute> attributes = PostProcessContainerRenderSubPass::GetCustomAttributes (rvc);

	/*
	 * Attach horizontal gaussian blur attributes to pipeline
	*/

	PipelineAttribute blurMapResolution;

	blurMapResolution.type = PipelineAttribute::AttrType::ATTR_2F;

	blurMapResolution.name = "blurMapResolution";

	blurMapResolution.value = glm::vec3 (GetPostProcessVolumeResolution (), 0.0f);

	attributes.push_back (blurMapResolution);

	return attributes;
}
