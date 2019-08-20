#include "SSAOContainerRenderSubPass.h"

#include "SSAOMapVolume.h"

bool SSAOContainerRenderSubPass::IsAvailable (const RenderScene* renderScene, const Camera* camera,
	const RenderSettings& settings, const RenderVolumeCollection* rvc) const
{
	/*
	 * Check if screen space ambient occlusion is enabled
	*/

	return settings.ssao_enabled;
}

std::string SSAOContainerRenderSubPass::GetPostProcessFragmentShaderPath () const
{
	return "Assets/Shaders/AmbientOcclusion/screenSpaceAmbientOcclusionFragment.glsl";
}

std::string SSAOContainerRenderSubPass::GetPostProcessVolumeName () const
{
	return "SSAOMapVolume";
}

glm::ivec2 SSAOContainerRenderSubPass::GetPostProcessVolumeResolution (const RenderSettings& settings) const
{
	return glm::ivec2 (glm::vec2 (settings.framebuffer.width, settings.framebuffer.height) * settings.ssao_scale);
}

PostProcessMapVolume* SSAOContainerRenderSubPass::CreatePostProcessVolume () const
{
	SSAOMapVolume* ssaoMapVolume = new SSAOMapVolume ();

	return ssaoMapVolume;
}

std::vector<PipelineAttribute> SSAOContainerRenderSubPass::GetCustomAttributes (const RenderSettings& settings, RenderVolumeCollection* rvc)
{
	/*
	 * Attach post process volume attributes to pipeline
	*/

	std::vector<PipelineAttribute> attributes = PostProcessContainerRenderSubPass::GetCustomAttributes (settings, rvc);

	/*
	 * Attach screen space ambient occlusion attributes to pipeline
	*/

	PipelineAttribute ssaoResolution;
	PipelineAttribute ssaoRadius;
	PipelineAttribute ssaoBias;

	ssaoResolution.type = PipelineAttribute::AttrType::ATTR_2F;
	ssaoRadius.type = PipelineAttribute::AttrType::ATTR_1F;
	ssaoBias.type = PipelineAttribute::AttrType::ATTR_1F;

	ssaoResolution.name = "ssaoResolution";
	ssaoRadius.name = "ssaoRadius";
	ssaoBias.name = "ssaoBias";

	glm::ivec2 resolution = glm::ivec2 (glm::vec2 (settings.framebuffer.width, settings.framebuffer.height) * settings.ssao_scale);

	ssaoResolution.value = glm::vec3 (resolution, 0.0f);
	ssaoRadius.value.x = settings.ssao_radius;
	ssaoBias.value.x = settings.ssao_bias;

	attributes.push_back (ssaoResolution);
	attributes.push_back (ssaoRadius);
	attributes.push_back (ssaoBias);

	return attributes;
}
