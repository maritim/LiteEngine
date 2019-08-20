#include "SSRContainerRenderSubPass.h"

#include "SSRMapVolume.h"

bool SSRContainerRenderSubPass::IsAvailable (const RenderScene* renderScene, const Camera* camera,
	const RenderSettings& settings, const RenderVolumeCollection* rvc) const
{
	/*
	 * Check if screen space reflection is enabled
	*/

	return settings.ssr_enabled;
}

std::string SSRContainerRenderSubPass::GetPostProcessFragmentShaderPath () const
{
	return "Assets/Shaders/ScreenSpaceReflection/screenSpaceReflectionFragment.glsl";
}

std::string SSRContainerRenderSubPass::GetPostProcessVolumeName () const
{
	return "SSRMapVolume";
}

glm::ivec2 SSRContainerRenderSubPass::GetPostProcessVolumeResolution (const RenderSettings& settings) const
{
	return glm::ivec2 (glm::vec2 (settings.framebuffer.width, settings.framebuffer.height) * settings.ssr_scale);
}

PostProcessMapVolume* SSRContainerRenderSubPass::CreatePostProcessVolume () const
{
	SSRMapVolume* ssrMapVolume = new SSRMapVolume ();

	return ssrMapVolume;
}

std::vector<PipelineAttribute> SSRContainerRenderSubPass::GetCustomAttributes (const RenderSettings& settings, RenderVolumeCollection* rvc)
{
	/*
	 * Attach post process volume attributes to pipeline
	*/

	std::vector<PipelineAttribute> attributes = PostProcessContainerRenderSubPass::GetCustomAttributes (settings, rvc);

	/*
	 * Attach screen space ambient occlusion attributes to pipeline
	*/

	PipelineAttribute ssrResolution;
	PipelineAttribute ssrIterations;
	PipelineAttribute ssrRoughness;
	PipelineAttribute ssrSampleSkip;
	PipelineAttribute ssrSpatialBias;

	ssrResolution.type = PipelineAttribute::AttrType::ATTR_2F;
	ssrIterations.type = PipelineAttribute::AttrType::ATTR_1I;
	ssrRoughness.type = PipelineAttribute::AttrType::ATTR_1F;
	ssrSampleSkip.type = PipelineAttribute::AttrType::ATTR_1F;
	ssrSpatialBias.type = PipelineAttribute::AttrType::ATTR_1F;

	ssrResolution.name = "ssrResolution";
	ssrIterations.name = "ssrIterations";
	ssrRoughness.name = "ssrRoughness";
	ssrSampleSkip.name = "ssrSampleSkip";
	ssrSpatialBias.name = "ssrSpatialBias";

	glm::ivec2 resolution = glm::ivec2 (glm::vec2 (settings.framebuffer.width, settings.framebuffer.height) * settings.ssr_scale);

	ssrResolution.value = glm::vec3 (resolution, 0.0f);
	ssrIterations.value.x = settings.ssr_iterations;
	ssrRoughness.value.x = settings.ssr_roughness;
	ssrSampleSkip.value.x = settings.ssr_sample_skip;
	ssrSpatialBias.value.x = settings.ssr_spatial_bias;

	attributes.push_back (ssrResolution);
	attributes.push_back (ssrIterations);
	attributes.push_back (ssrRoughness);
	attributes.push_back (ssrSampleSkip);
	attributes.push_back (ssrSpatialBias);

	return attributes;
}
