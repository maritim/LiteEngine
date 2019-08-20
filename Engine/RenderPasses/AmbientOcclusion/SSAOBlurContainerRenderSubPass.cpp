#include "SSAOBlurContainerRenderSubPass.h"

#include "SSAOMapVolume.h"

bool SSAOBlurContainerRenderSubPass::IsAvailable (const RenderScene* renderScene, const Camera* camera,
	const RenderSettings& settings, const RenderVolumeCollection* rvc) const
{
	/*
	 * Check if screen space ambient occlusion blur is enabled
	*/

	return settings.ssao_enabled && settings.ssao_blur_enabled;
}

std::string SSAOBlurContainerRenderSubPass::GetPostProcessFragmentShaderPath () const
{
	return "Assets/Shaders/AmbientOcclusion/screenSpaceAmbientOcclusionBlurFragment.glsl";
}

std::string SSAOBlurContainerRenderSubPass::GetPostProcessVolumeName () const
{
	return "SSAOMapVolume";
}

glm::ivec2 SSAOBlurContainerRenderSubPass::GetPostProcessVolumeResolution (const RenderSettings& settings) const
{
	return glm::ivec2 (glm::vec2 (settings.framebuffer.width, settings.framebuffer.height) * settings.ssao_scale);
}

PostProcessMapVolume* SSAOBlurContainerRenderSubPass::CreatePostProcessVolume () const
{
	SSAOMapVolume* ssaoBlurMapVolume = new SSAOMapVolume ();

	return ssaoBlurMapVolume;
}

std::vector<PipelineAttribute> SSAOBlurContainerRenderSubPass::GetCustomAttributes (const RenderSettings& settings, RenderVolumeCollection* rvc)
{
	/*
	 * Attach post process volume attributes to pipeline
	*/

	std::vector<PipelineAttribute> attributes = PostProcessContainerRenderSubPass::GetCustomAttributes (settings, rvc);

	/*
	 * Attach screen space ambient occlusion attributes to pipeline
	*/

	PipelineAttribute ssaoResolution;

	ssaoResolution.type = PipelineAttribute::AttrType::ATTR_2F;

	ssaoResolution.name = "ssaoResolution";

	glm::ivec2 resolution = glm::ivec2 (glm::vec2 (settings.framebuffer.width, settings.framebuffer.height) * settings.ssao_scale);

	ssaoResolution.value = glm::vec3 (resolution, 0.0f);

	attributes.push_back (ssaoResolution);

	return attributes;
}
