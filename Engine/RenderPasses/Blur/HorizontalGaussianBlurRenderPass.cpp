#include "HorizontalGaussianBlurRenderPass.h"

#include "BlurMapVolume.h"

std::string HorizontalGaussianBlurRenderPass::GetPostProcessFragmentShaderPath () const
{
	return "Assets/Shaders/Blur/horizontalGaussianBlurFragment.glsl";
}

std::string HorizontalGaussianBlurRenderPass::GetPostProcessVolumeName () const
{
	return "BlurMapVolume";
}

PostProcessMapVolume* HorizontalGaussianBlurRenderPass::CreatePostProcessVolume () const
{
	BlurMapVolume* blurMapVolume = new BlurMapVolume ();

	return blurMapVolume;
}

std::vector<PipelineAttribute> HorizontalGaussianBlurRenderPass::GetCustomAttributes (const RenderSettings& settings, RenderVolumeCollection* rvc)
{
	/*
	 * Attach post process volume attributes to pipeline
	*/

	std::vector<PipelineAttribute> attributes = PostProcessRenderPass::GetCustomAttributes (settings, rvc);

	/*
	 * Attach horizontal gaussian blur attributes to pipeline
	*/

	PipelineAttribute blurMapResolution;

	blurMapResolution.type = PipelineAttribute::AttrType::ATTR_2F;

	blurMapResolution.name = "blurMapResolution";

	blurMapResolution.value = glm::vec3 (GetPostProcessVolumeResolution (settings), 0.0f);

	attributes.push_back (blurMapResolution);

	return attributes;
}
