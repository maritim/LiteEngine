#include "BloomAccumulationRenderPass.h"

bool BloomAccumulationRenderPass::IsAvailable (const RenderScene* renderScene, const Camera* camera,
	const RenderSettings& settings, const RenderVolumeCollection* rvc) const
{
	/*
	 * Check if bloom is enabled
	*/

	return settings.bloom_enabled;
}

std::string BloomAccumulationRenderPass::GetPostProcessFragmentShaderPath () const
{
	return "Assets/Shaders/Bloom/bloomAccumulationFragment.glsl";
}

std::string BloomAccumulationRenderPass::GetPostProcessVolumeName () const
{
	return "PostProcessMapVolume";
}

glm::ivec2 BloomAccumulationRenderPass::GetPostProcessVolumeResolution (const RenderSettings& settings) const
{
	return glm::ivec2 (settings.framebuffer.width, settings.framebuffer.height);
}

PostProcessMapVolume* BloomAccumulationRenderPass::CreatePostProcessVolume () const
{
	PostProcessMapVolume* volume = new PostProcessMapVolume ();

	return volume;
}

std::vector<PipelineAttribute> BloomAccumulationRenderPass::GetCustomAttributes (const Camera* camera,
	const RenderSettings& settings, RenderVolumeCollection* rvc)
{
	/*
	 * Attach post process volume attributes to pipeline
	*/

	std::vector<PipelineAttribute> attributes = PostProcessRenderPass::GetCustomAttributes (camera, settings, rvc);

	/*
	 * Attach bloom attributes to pipeline
	*/

	PipelineAttribute bloomIntensity;

	bloomIntensity.type = PipelineAttribute::AttrType::ATTR_1F;

	bloomIntensity.name = "bloomIntensity";

	bloomIntensity.value.x = settings.bloom_intensity;

	attributes.push_back (bloomIntensity);

	return attributes;
}
