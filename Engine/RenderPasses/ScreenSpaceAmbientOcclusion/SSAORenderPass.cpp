#include "SSAORenderPass.h"

#include "Debug/Statistics/StatisticsManager.h"
#include "SSAOStatisticsObject.h"

bool SSAORenderPass::IsAvailable (const RenderScene* renderScene, const Camera* camera,
	const RenderSettings& settings, const RenderVolumeCollection* rvc) const
{
	/*
	 * Check if screen space ambient occlusion is enabled
	*/

	return settings.ssao_enabled;
}

std::string SSAORenderPass::GetPostProcessFragmentShaderPath () const
{
	return "Assets/Shaders/ScreenSpaceAmbientOcclusion/screenSpaceAmbientOcclusionFragment.glsl";
}

std::string SSAORenderPass::GetPostProcessVolumeName () const
{
	return "SSAOMapVolume";
}

glm::ivec2 SSAORenderPass::GetPostProcessVolumeResolution (const RenderSettings& settings) const
{
	return glm::ivec2 (glm::vec2 (settings.resolution.width, settings.resolution.height) * settings.ssao_scale);
}

FramebufferRenderVolume* SSAORenderPass::CreatePostProcessVolume (const RenderSettings& settings) const
{
	/*
	 * Create screen space ambient occlusion framebuffer
	*/

	Resource<Texture> texture = Resource<Texture> (new Texture ("ambientOcclusionMap"));

	glm::ivec2 size = GetPostProcessVolumeResolution (settings);

	texture->SetSize (Size (size.x, size.y));
	texture->SetMipmapGeneration (false);
	texture->SetSizedInternalFormat (TEXTURE_SIZED_INTERNAL_FORMAT::FORMAT_R8);
	texture->SetInternalFormat (TEXTURE_INTERNAL_FORMAT::FORMAT_RED);
	texture->SetChannelType (TEXTURE_CHANNEL_TYPE::CHANNEL_UNSIGNED_BYTE);
	texture->SetWrapMode (TEXTURE_WRAP_MODE::WRAP_CLAMP_EDGE);
	texture->SetMinFilter (TEXTURE_FILTER_MODE::FILTER_NEAREST);
	texture->SetMagFilter (TEXTURE_FILTER_MODE::FILTER_NEAREST);
	texture->SetAnisotropicFiltering (false);

	Resource<Framebuffer> framebuffer = Resource<Framebuffer> (new Framebuffer (texture));

	FramebufferRenderVolume* renderVolume = new FramebufferRenderVolume (framebuffer);

	/*
	 * Update statistics object
	*/

	auto ssaoStatisticsObject = StatisticsManager::Instance ()->GetStatisticsObject <SSAOStatisticsObject> ();

	ssaoStatisticsObject->ssaoMapVolume = renderVolume;

	return renderVolume;
}

std::vector<PipelineAttribute> SSAORenderPass::GetCustomAttributes (const Camera* camera,
	const RenderSettings& settings, RenderVolumeCollection* rvc)
{
	/*
	 * Attach post process volume attributes to pipeline
	*/

	std::vector<PipelineAttribute> attributes = PostProcessRenderPass::GetCustomAttributes (camera, settings, rvc);

	/*
	 * Attach screen space ambient occlusion attributes to pipeline
	*/

	PipelineAttribute ssaoResolution;
	PipelineAttribute ssaoRadius;
	PipelineAttribute ssaoBias;
	PipelineAttribute ssaoTemporalFilter;

	ssaoResolution.type = PipelineAttribute::AttrType::ATTR_2F;
	ssaoRadius.type = PipelineAttribute::AttrType::ATTR_1F;
	ssaoBias.type = PipelineAttribute::AttrType::ATTR_1F;
	ssaoTemporalFilter.type = PipelineAttribute::AttrType::ATTR_1I;

	ssaoResolution.name = "ssaoResolution";
	ssaoRadius.name = "ssaoRadius";
	ssaoBias.name = "ssaoBias";
	ssaoTemporalFilter.name = "ssaoTemporalFilter";

	glm::ivec2 resolution = glm::ivec2 (glm::vec2 (settings.resolution.width, settings.resolution.height) * settings.ssao_scale);

	ssaoResolution.value = glm::vec3 (resolution, 0.0f);
	ssaoRadius.value.x = settings.ssao_radius;
	ssaoBias.value.x = settings.ssao_bias;
	ssaoTemporalFilter.value.x = settings.ssao_temporal_filter_enabled;

	attributes.push_back (ssaoResolution);
	attributes.push_back (ssaoRadius);
	attributes.push_back (ssaoBias);
	attributes.push_back (ssaoTemporalFilter);

	return attributes;
}
