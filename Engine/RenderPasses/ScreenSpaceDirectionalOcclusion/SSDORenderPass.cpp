#include "SSDORenderPass.h"

#include "Wrappers/OpenGL/GL.h"

#include "Debug/Statistics/StatisticsManager.h"
#include "SSDOStatisticsObject.h"

#include "Utils/Extensions/MathExtend.h"

bool SSDORenderPass::IsAvailable (const RenderScene* renderScene, const Camera* camera,
	const RenderSettings& settings, const RenderVolumeCollection* rvc) const
{
	/*
	 * Always execute screen space directional occlusion render pass
	*/

	return settings.ssdo_enabled;
}

std::string SSDORenderPass::GetPostProcessFragmentShaderPath () const
{
	return "Assets/Shaders/ScreenSpaceDirectionalOcclusion/screenSpaceDirectionalOcclusionFragment.glsl";
}

std::string SSDORenderPass::GetPostProcessVolumeName () const
{
	return "SSDOMapVolume";
}

glm::ivec2 SSDORenderPass::GetPostProcessVolumeResolution (const RenderSettings& settings) const
{
	return glm::ivec2 (glm::vec2 (settings.resolution.width, settings.resolution.height) * settings.ssdo_scale);
}

FramebufferRenderVolume* SSDORenderPass::CreatePostProcessVolume (const RenderSettings& settings) const
{
	/*
	 * Create screen space directional occlusion framebuffer
	*/

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

	ssdoStatisticsObject->ssdoMapVolume = renderVolume;

	return renderVolume;
}

std::vector<PipelineAttribute> SSDORenderPass::GetCustomAttributes (const Camera* camera,
	const RenderSettings& settings, RenderVolumeCollection* rvc)
{
	/*
	 * Attach post process volume attributes to pipeline
	*/

	std::vector<PipelineAttribute> attributes = PostProcessRenderPass::GetCustomAttributes (camera, settings, rvc);

	/*
	 * Attach screen space directional occlusion attributes to pipeline
	*/

	PipelineAttribute ssdoResolution;
	PipelineAttribute ssdoRadius;
	PipelineAttribute ssdoBias;
	PipelineAttribute ssdoSamplingScale;
	PipelineAttribute ssdoIndirectIntensity;
	PipelineAttribute ssdoTemporalFilter;
	PipelineAttribute ssdoInterpolationEnabled;
	PipelineAttribute ssdoInterpolationScale;
	PipelineAttribute ssdoMinInterpolationDistance;
	PipelineAttribute ssdoMinInterpolationAngle;
	PipelineAttribute ssdoDebugInterpolation;

	ssdoResolution.type = PipelineAttribute::AttrType::ATTR_2F;
	ssdoRadius.type = PipelineAttribute::AttrType::ATTR_1F;
	ssdoBias.type = PipelineAttribute::AttrType::ATTR_1F;
	ssdoSamplingScale.type = PipelineAttribute::AttrType::ATTR_1F;
	ssdoIndirectIntensity.type = PipelineAttribute::AttrType::ATTR_1F;
	ssdoTemporalFilter.type = PipelineAttribute::AttrType::ATTR_1I;
	ssdoInterpolationEnabled.type = PipelineAttribute::AttrType::ATTR_1I;
	ssdoInterpolationScale.type = PipelineAttribute::AttrType::ATTR_1F;
	ssdoMinInterpolationDistance.type = PipelineAttribute::AttrType::ATTR_1F;
	ssdoMinInterpolationAngle.type = PipelineAttribute::AttrType::ATTR_1F;
	ssdoDebugInterpolation.type = PipelineAttribute::AttrType::ATTR_1I;

	ssdoResolution.name = "ssdoResolution";
	ssdoRadius.name = "ssdoRadius";
	ssdoBias.name = "ssdoBias";
	ssdoSamplingScale.name = "ssdoSamplingScale";
	ssdoIndirectIntensity.name = "ssdoIndirectIntensity";
	ssdoTemporalFilter.name = "ssdoTemporalFilter";
	ssdoInterpolationEnabled.name = "ssdoInterpolationEnabled";
	ssdoInterpolationScale.name = "ssdoInterpolationScale";
	ssdoMinInterpolationDistance.name = "ssdoMinInterpolationDistance";
	ssdoMinInterpolationAngle.name = "ssdoMinInterpolationAngle";
	ssdoDebugInterpolation.name = "ssdoDebugInterpolation";

	glm::ivec2 resolution = glm::ivec2 (glm::vec2 (settings.resolution.width, settings.resolution.height) * settings.ssdo_scale);

	ssdoResolution.value = glm::vec3 (resolution, 0.0f);
	ssdoRadius.value.x = settings.ssdo_radius;
	ssdoBias.value.x = settings.ssdo_bias;
	ssdoSamplingScale.value.x = settings.ssdo_sampling_scale;
	ssdoIndirectIntensity.value.x = settings.ssdo_indirect_intensity;
	ssdoTemporalFilter.value.x = settings.ssdo_temporal_filter_enabled;
	ssdoInterpolationEnabled.value.x = settings.ssdo_interpolation_enabled;
	ssdoInterpolationScale.value.x = settings.ssdo_interpolation_scale;
	ssdoMinInterpolationDistance.value.x = settings.ssdo_min_interpolation_distance;
	ssdoMinInterpolationAngle.value.x = std::cos (DEG2RAD * settings.ssdo_min_interpolation_angle);
	ssdoDebugInterpolation.value.x = settings.ssdo_debug_interpolation;

	attributes.push_back (ssdoResolution);
	attributes.push_back (ssdoRadius);
	attributes.push_back (ssdoBias);
	attributes.push_back (ssdoSamplingScale);
	attributes.push_back (ssdoIndirectIntensity);
	attributes.push_back (ssdoTemporalFilter);
	attributes.push_back (ssdoInterpolationEnabled);
	attributes.push_back (ssdoInterpolationScale);
	attributes.push_back (ssdoMinInterpolationDistance);
	attributes.push_back (ssdoMinInterpolationAngle);
	attributes.push_back (ssdoDebugInterpolation);

	return attributes;
}
