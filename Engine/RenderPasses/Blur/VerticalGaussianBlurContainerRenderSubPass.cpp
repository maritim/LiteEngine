#include "VerticalGaussianBlurContainerRenderSubPass.h"

#include "BlurMapVolume.h"

std::string VerticalGaussianBlurContainerRenderSubPass::GetPostProcessFragmentShaderPath () const
{
	return "Assets/Shaders/Blur/verticalGaussianBlurFragment.glsl";
}

std::string VerticalGaussianBlurContainerRenderSubPass::GetPostProcessVolumeName () const
{
	return "BlurMapVolume";
}

PostProcessMapVolume* VerticalGaussianBlurContainerRenderSubPass::CreatePostProcessVolume () const
{
	BlurMapVolume* blurMapVolume = new BlurMapVolume ();

	return blurMapVolume;
}

std::vector<PipelineAttribute> VerticalGaussianBlurContainerRenderSubPass::GetCustomAttributes (RenderVolumeCollection* rvc)
{
	/*
	 * Attach post process volume attributes to pipeline
	*/

	std::vector<PipelineAttribute> attributes = PostProcessContainerRenderSubPass::GetCustomAttributes (rvc);

	/*
	 * Attach vertical gaussian blur attributes to pipeline
	*/

	PipelineAttribute blurMapResolution;

	blurMapResolution.type = PipelineAttribute::AttrType::ATTR_2F;

	blurMapResolution.name = "blurMapResolution";

	blurMapResolution.value = glm::vec3 (GetPostProcessVolumeResolution (), 0.0f);

	attributes.push_back (blurMapResolution);

	return attributes;
}
