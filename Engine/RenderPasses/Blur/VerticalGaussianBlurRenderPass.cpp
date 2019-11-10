#include "VerticalGaussianBlurRenderPass.h"

#include "BlurMapVolume.h"

std::string VerticalGaussianBlurRenderPass::GetPostProcessFragmentShaderPath () const
{
	return "Assets/Shaders/Blur/verticalGaussianBlurFragment.glsl";
}

std::string VerticalGaussianBlurRenderPass::GetPostProcessVolumeName () const
{
	return "BlurMapVolume";
}

PostProcessMapVolume* VerticalGaussianBlurRenderPass::CreatePostProcessVolume () const
{
	BlurMapVolume* blurMapVolume = new BlurMapVolume ();

	return blurMapVolume;
}

std::vector<PipelineAttribute> VerticalGaussianBlurRenderPass::GetCustomAttributes (const Camera* camera,
	const RenderSettings& settings, RenderVolumeCollection* rvc)
{
	/*
	 * Attach post process volume attributes to pipeline
	*/

	std::vector<PipelineAttribute> attributes = PostProcessRenderPass::GetCustomAttributes (camera, settings, rvc);

	/*
	 * Attach vertical gaussian blur attributes to pipeline
	*/

	PipelineAttribute blurMapResolution;

	blurMapResolution.type = PipelineAttribute::AttrType::ATTR_2F;

	blurMapResolution.name = "blurMapResolution";

	blurMapResolution.value = glm::vec3 (GetPostProcessVolumeResolution (settings), 0.0f);

	attributes.push_back (blurMapResolution);

	return attributes;
}
