#include "SSRAccumulationContainerRenderSubPass.h"

bool SSRAccumulationContainerRenderSubPass::IsAvailable (const Scene* scene, const Camera* camera,
	const RenderSettings& settings, const RenderVolumeCollection* rvc) const
{
	/*
	 * Check if screen space ambient occlusion is enabled
	*/

	return settings.ssr_enabled;
}

std::string SSRAccumulationContainerRenderSubPass::GetPostProcessFragmentShaderPath () const
{
	return "Assets/Shaders/ScreenSpaceReflection/screenSpaceReflectionAccumulationFragment.glsl";
}

std::string SSRAccumulationContainerRenderSubPass::GetPostProcessVolumeName () const
{
	return "PostProcessMapVolume";
}

glm::ivec2 SSRAccumulationContainerRenderSubPass::GetPostProcessVolumeResolution (const RenderSettings& settings) const
{
	return glm::ivec2 (settings.framebuffer.width, settings.framebuffer.height);
}

PostProcessMapVolume* SSRAccumulationContainerRenderSubPass::CreatePostProcessVolume () const
{
	PostProcessMapVolume* postProcessMapVolume = new PostProcessMapVolume ();

	return postProcessMapVolume;
}

std::vector<PipelineAttribute> SSRAccumulationContainerRenderSubPass::GetCustomAttributes (const RenderSettings& settings, RenderVolumeCollection* rvc)
{
	/*
	 * Attach post process volume attributes to pipeline
	*/

	std::vector<PipelineAttribute> attributes = PostProcessContainerRenderSubPass::GetCustomAttributes (settings, rvc);

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
