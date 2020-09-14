#include "SSDOInterpolatedRenderPass.h"

#include "Debug/Statistics/StatisticsManager.h"
#include "SSDOStatisticsObject.h"

bool SSDOInterpolatedRenderPass::IsAvailable (const RenderScene* renderScene, const Camera* camera,
	const RenderSettings& settings, const RenderVolumeCollection* rvc) const
{
	/*
	 * Always execure reflective shadow mapping indirect light render pass
	*/

	return settings.ssdo_enabled && settings.ssdo_interpolation_enabled;
}

std::string SSDOInterpolatedRenderPass::GetPostProcessFragmentShaderPath () const
{
	return "Assets/Shaders/ScreenSpaceDirectionalOcclusion/screenSpaceDirectionalOcclusionInterpolatedFragment.glsl";
}

std::string SSDOInterpolatedRenderPass::GetPostProcessVolumeName () const
{
	return "SSDOMapVolume";
}

glm::ivec2 SSDOInterpolatedRenderPass::GetPostProcessVolumeResolution (const RenderSettings& settings) const
{
	return glm::ivec2 (glm::vec2 (settings.resolution.width, settings.resolution.height) * settings.ssdo_interpolation_scale);
}

FramebufferRenderVolume* SSDOInterpolatedRenderPass::CreatePostProcessVolume (const RenderSettings& settings) const
{
	/*
	 * Create rsm interpolated indirect diffuse light framebuffer
	*/

	Resource<Texture> texture = Resource<Texture> (new Texture ("ssdoMap"));

	glm::ivec2 size = GetPostProcessVolumeResolution (settings);

	texture->SetSize (Size (size.x, size.y));
	texture->SetMipmapGeneration (false);
	texture->SetSizedInternalFormat (TEXTURE_SIZED_INTERNAL_FORMAT::FORMAT_RGB16);
	texture->SetInternalFormat (TEXTURE_INTERNAL_FORMAT::FORMAT_RGB);
	texture->SetChannelType (TEXTURE_CHANNEL_TYPE::CHANNEL_FLOAT);
	texture->SetWrapMode (TEXTURE_WRAP_MODE::WRAP_CLAMP_EDGE);
	texture->SetMinFilter (TEXTURE_FILTER_MODE::FILTER_LINEAR);
	texture->SetMagFilter (TEXTURE_FILTER_MODE::FILTER_LINEAR);
	texture->SetAnisotropicFiltering (false);

	Resource<Framebuffer> framebuffer = Resource<Framebuffer> (new Framebuffer (texture));

	FramebufferRenderVolume* renderVolume = new FramebufferRenderVolume (framebuffer);

	/*
	 * Update statistics object
	*/

	auto ssdoStatisticsObject = StatisticsManager::Instance ()->GetStatisticsObject <SSDOStatisticsObject> ();

	ssdoStatisticsObject->ssdoInterpolatedMapVolume = renderVolume;

	return renderVolume;
}

std::vector<PipelineAttribute> SSDOInterpolatedRenderPass::GetCustomAttributes (const Camera* camera,
	const RenderSettings& settings, RenderVolumeCollection* rvc)
{
	/*
	 * Attach post process volume attributes to pipeline
	*/

	std::vector<PipelineAttribute> attributes = PostProcessRenderPass::GetCustomAttributes (camera, settings, rvc);

	/*
	 * Attach screen space ambient occlusion attributes to pipeline
	*/

	PipelineAttribute ssdoResolution;
	PipelineAttribute ssdoRadius;
	PipelineAttribute ssdoBias;
	PipelineAttribute ssdoIndirectIntensity;
	PipelineAttribute ssdoTemporalFilter;
	PipelineAttribute ssdoInterpolationScale;

	ssdoResolution.type = PipelineAttribute::AttrType::ATTR_2F;
	ssdoRadius.type = PipelineAttribute::AttrType::ATTR_1F;
	ssdoBias.type = PipelineAttribute::AttrType::ATTR_1F;
	ssdoIndirectIntensity.type = PipelineAttribute::AttrType::ATTR_1F;
	ssdoTemporalFilter.type = PipelineAttribute::AttrType::ATTR_1I;
	ssdoInterpolationScale.type = PipelineAttribute::AttrType::ATTR_1F;

	ssdoResolution.name = "ssdoResolution";
	ssdoRadius.name = "ssdoRadius";
	ssdoBias.name = "ssdoBias";
	ssdoIndirectIntensity.name = "ssdoIndirectIntensity";
	ssdoTemporalFilter.name = "ssdoTemporalFilter";
	ssdoInterpolationScale.name = "ssdoInterpolationScale";

	glm::ivec2 resolution = glm::ivec2 (glm::vec2 (settings.resolution.width, settings.resolution.height) * settings.ssdo_interpolation_scale);

	ssdoResolution.value = glm::vec3 (resolution, 0.0f);
	ssdoRadius.value.x = settings.ssdo_radius;
	ssdoBias.value.x = settings.ssdo_bias;
	ssdoIndirectIntensity.value.x = settings.ssdo_indirect_intensity;
	ssdoTemporalFilter.value.x = settings.ssdo_temporal_filter_enabled;
	ssdoInterpolationScale.value.x = settings.ssdo_interpolation_scale;

	attributes.push_back (ssdoResolution);
	attributes.push_back (ssdoRadius);
	attributes.push_back (ssdoBias);
	attributes.push_back (ssdoIndirectIntensity);
	attributes.push_back (ssdoTemporalFilter);
	attributes.push_back (ssdoInterpolationScale);

	return attributes;
}
