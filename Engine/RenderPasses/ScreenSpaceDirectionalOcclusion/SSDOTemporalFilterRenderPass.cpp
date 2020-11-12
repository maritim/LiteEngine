#include "SSDOTemporalFilterRenderPass.h"

#include "Debug/Statistics/StatisticsManager.h"
#include "SSDOStatisticsObject.h"

bool SSDOTemporalFilterRenderPass::IsAvailable (const RenderScene* renderScene, const Camera* camera,
	const RenderSettings& settings, const RenderVolumeCollection* rvc) const
{
	/*
	 * Always execure reflective shadow mapping indirect light render pass
	*/

	return settings.ssdo_enabled && settings.ssdo_temporal_filter_enabled;
}

std::string SSDOTemporalFilterRenderPass::GetPostProcessFragmentShaderPath () const
{
	return "Assets/Shaders/ScreenSpaceDirectionalOcclusion/screenSpaceDirectionalOcclusionTemporalFilterFragment.glsl";
}

std::string SSDOTemporalFilterRenderPass::GetPostProcessVolumeName () const
{
	return "SSDOMapVolume";
}

glm::ivec2 SSDOTemporalFilterRenderPass::GetPostProcessVolumeResolution (const RenderSettings& settings) const
{
	return glm::ivec2 (glm::vec2 (settings.resolution.width, settings.resolution.height) * settings.ssdo_scale);
}

FramebufferRenderVolume* SSDOTemporalFilterRenderPass::CreatePostProcessVolume (const RenderSettings& settings) const
{
	Resource<Texture> texture = Resource<Texture> (new Texture ("ssdoMap"));

	glm::ivec2 size = GetPostProcessVolumeResolution (settings);

	texture->SetSize (Size (size.x, size.y));
	texture->SetMipmapGeneration (false);
	texture->SetSizedInternalFormat (TEXTURE_SIZED_INTERNAL_FORMAT::FORMAT_RGB16);
	texture->SetInternalFormat (TEXTURE_INTERNAL_FORMAT::FORMAT_RGB);
	texture->SetChannelType (TEXTURE_CHANNEL_TYPE::CHANNEL_FLOAT);
	texture->SetWrapMode (TEXTURE_WRAP_MODE::WRAP_CLAMP_EDGE);
	texture->SetMinFilter (TEXTURE_FILTER_MODE::FILTER_NEAREST);
	texture->SetMagFilter (TEXTURE_FILTER_MODE::FILTER_NEAREST);
	texture->SetAnisotropicFiltering (false);

	Resource<Framebuffer> framebuffer = Resource<Framebuffer> (new Framebuffer (texture));

	FramebufferRenderVolume* renderVolume = new FramebufferRenderVolume (framebuffer);

	/*
	 * Update statistics object
	*/

	auto ssdoStatisticsObject = StatisticsManager::Instance ()->GetStatisticsObject <SSDOStatisticsObject> ();

	ssdoStatisticsObject->ssdoTemporalFilterMapVolume = renderVolume;

	return renderVolume;
}

std::vector<PipelineAttribute> SSDOTemporalFilterRenderPass::GetCustomAttributes (const Camera* camera,
	const RenderSettings& settings, RenderVolumeCollection* rvc)
{
	/*
	 * Attach post process volume attributes to pipeline
	*/

	std::vector<PipelineAttribute> attributes = TemporalFilterRenderPass::GetCustomAttributes (camera, settings, rvc);

	/*
	 * Attach screen space directional occlusion attributes to pipeline
	*/

	PipelineAttribute ssdoResolution;

	ssdoResolution.type = PipelineAttribute::AttrType::ATTR_2F;

	ssdoResolution.name = "ssdoResolution";

	glm::ivec2 resolution = glm::ivec2 (glm::vec2 (settings.resolution.width, settings.resolution.height) * settings.ssdo_scale);

	ssdoResolution.value = glm::vec3 (resolution, 0.0f);

	attributes.push_back (ssdoResolution);

	return attributes;
}
