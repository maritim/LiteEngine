#include "HDRContainerRenderSubPass.h"

bool HDRContainerRenderSubPass::IsAvailable (const RenderScene* renderScene, const Camera* camera,
	const RenderSettings& settings, const RenderVolumeCollection* rvc) const
{
	/*
	 * Check if high dynamic range is enabled
	*/

	return settings.hdr_enabled;
}

std::string HDRContainerRenderSubPass::GetPostProcessFragmentShaderPath () const
{
	return "Assets/Shaders/HighDynamicRange/highDynamicRangeFragment.glsl";
}

std::string HDRContainerRenderSubPass::GetPostProcessVolumeName () const
{
	return "PostProcessMapVolume";
}

glm::ivec2 HDRContainerRenderSubPass::GetPostProcessVolumeResolution (const RenderSettings& settings) const
{
	return glm::ivec2 (settings.framebuffer.width, settings.framebuffer.height);
}

PostProcessMapVolume* HDRContainerRenderSubPass::CreatePostProcessVolume () const
{
	PostProcessMapVolume* volume = new PostProcessMapVolume ();

	return volume;
}

std::vector<PipelineAttribute> HDRContainerRenderSubPass::GetCustomAttributes (const RenderSettings& settings, RenderVolumeCollection* rvc)
{
	/*
	 * Attach post process volume attributes to pipeline
	*/

	std::vector<PipelineAttribute> attributes = PostProcessContainerRenderSubPass::GetCustomAttributes (settings, rvc);

	/*
	 * Attach high dynamic range attributes to pipeline
	*/

	PipelineAttribute exposure;

	exposure.type = PipelineAttribute::AttrType::ATTR_1F;

	exposure.name = "exposure";

	exposure.value.x = settings.hdr_exposure;

	attributes.push_back (exposure);

	return attributes;
}
