#include "SSRAccumulationRenderPass.h"

bool SSRAccumulationRenderPass::IsAvailable (const RenderScene* renderScene, const Camera* camera,
	const RenderSettings& settings, const RenderVolumeCollection* rvc) const
{
	/*
	 * Check if screen space ambient occlusion is enabled
	*/

	return settings.ssr_enabled;
}

std::string SSRAccumulationRenderPass::GetPostProcessFragmentShaderPath () const
{
	return "Assets/Shaders/ScreenSpaceReflection/screenSpaceReflectionAccumulationFragment.glsl";
}

std::string SSRAccumulationRenderPass::GetPostProcessVolumeName () const
{
	return "PostProcessMapVolume";
}

glm::ivec2 SSRAccumulationRenderPass::GetPostProcessVolumeResolution (const RenderSettings& settings) const
{
	return glm::ivec2 (settings.framebuffer.width, settings.framebuffer.height);
}

PostProcessMapVolume* SSRAccumulationRenderPass::CreatePostProcessVolume () const
{
	PostProcessMapVolume* postProcessMapVolume = new PostProcessMapVolume ();

	return postProcessMapVolume;
}

std::vector<PipelineAttribute> SSRAccumulationRenderPass::GetCustomAttributes (const RenderSettings& settings, RenderVolumeCollection* rvc)
{
	/*
	 * Attach post process volume attributes to pipeline
	*/

	std::vector<PipelineAttribute> attributes = PostProcessRenderPass::GetCustomAttributes (settings, rvc);

	/*
	 * Attach screen space ambient occlusion attributes to pipeline
	*/

	PipelineAttribute ssrIntensity;

	ssrIntensity.type = PipelineAttribute::AttrType::ATTR_1F;

	ssrIntensity.name = "ssrIntensity";

	ssrIntensity.value.x = settings.ssr_intensity;

	attributes.push_back (ssrIntensity);

	return attributes;
}
