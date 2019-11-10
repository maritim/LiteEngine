#include "HDRRenderPass.h"

bool HDRRenderPass::IsAvailable (const RenderScene* renderScene, const Camera* camera,
	const RenderSettings& settings, const RenderVolumeCollection* rvc) const
{
	/*
	 * Check if high dynamic range is enabled
	*/

	return settings.hdr_enabled;
}

std::string HDRRenderPass::GetPostProcessFragmentShaderPath () const
{
	return "Assets/Shaders/HighDynamicRange/highDynamicRangeFragment.glsl";
}

std::string HDRRenderPass::GetPostProcessVolumeName () const
{
	return "PostProcessMapVolume";
}

glm::ivec2 HDRRenderPass::GetPostProcessVolumeResolution (const RenderSettings& settings) const
{
	return glm::ivec2 (settings.framebuffer.width, settings.framebuffer.height);
}

PostProcessMapVolume* HDRRenderPass::CreatePostProcessVolume () const
{
	PostProcessMapVolume* volume = new PostProcessMapVolume ();

	return volume;
}

std::vector<PipelineAttribute> HDRRenderPass::GetCustomAttributes (const Camera* camera,
	const RenderSettings& settings, RenderVolumeCollection* rvc)
{
	/*
	 * Attach post process volume attributes to pipeline
	*/

	std::vector<PipelineAttribute> attributes = PostProcessRenderPass::GetCustomAttributes (camera, settings, rvc);

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
