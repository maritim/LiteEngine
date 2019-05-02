#include "BrightExtractionContainerRenderSubPass.h"

#include "RenderPasses/Blur/BlurMapVolume.h"

bool BrightExtractionContainerRenderSubPass::IsAvailable (const Scene* scene, const Camera* camera,
	const RenderSettings& settings, const RenderVolumeCollection* rvc) const
{
	/*
	 * Check if bloom is enabled
	*/

	return settings.bloom_enabled;
}

std::string BrightExtractionContainerRenderSubPass::GetPostProcessFragmentShaderPath () const
{
	return "Assets/Shaders/Bloom/brightExtractionFragment.glsl";
}

std::string BrightExtractionContainerRenderSubPass::GetPostProcessVolumeName () const
{
	return "BlurMapVolume";
}

glm::ivec2 BrightExtractionContainerRenderSubPass::GetPostProcessVolumeResolution (const RenderSettings& settings) const
{
	return glm::ivec2 (glm::vec2 (settings.framebuffer.width, settings.framebuffer.height) * settings.bloom_scale);
}

PostProcessMapVolume* BrightExtractionContainerRenderSubPass::CreatePostProcessVolume () const
{
	BlurMapVolume* blurMapVolume = new BlurMapVolume ();

	return blurMapVolume;
}

std::vector<PipelineAttribute> BrightExtractionContainerRenderSubPass::GetCustomAttributes (const RenderSettings& settings, RenderVolumeCollection* rvc)
{
	/*
	 * Attach post process volume attributes to pipeline
	*/

	std::vector<PipelineAttribute> attributes = PostProcessContainerRenderSubPass::GetCustomAttributes (settings, rvc);

	/*
	 * Attach bloom attributes to pipeline
	*/

	PipelineAttribute bloomResolution;
	PipelineAttribute bloomThreshold;

	bloomResolution.type = PipelineAttribute::AttrType::ATTR_2F;
	bloomThreshold.type = PipelineAttribute::AttrType::ATTR_1F;

	bloomResolution.name = "bloomResolution";
	bloomThreshold.name = "bloomThreshold";

	glm::ivec2 resolution = glm::ivec2 (glm::vec2 (settings.framebuffer.width, settings.framebuffer.height) * settings.bloom_scale);

	bloomResolution.value = glm::vec3 (resolution, 0.0f);
	bloomThreshold.value.x = settings.bloom_threshold;

	attributes.push_back (bloomResolution);
	attributes.push_back (bloomThreshold);

	return attributes;
}
