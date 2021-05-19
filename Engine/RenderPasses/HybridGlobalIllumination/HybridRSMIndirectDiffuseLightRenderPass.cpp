#include "HybridRSMIndirectDiffuseLightRenderPass.h"

#include "Debug/Statistics/StatisticsManager.h"
#include "HGIStatisticsObject.h"

#include "Utils/Extensions/MathExtend.h"

bool HybridRSMIndirectDiffuseLightRenderPass::IsAvailable (const RenderScene* renderScene, const Camera* camera,
	const RenderSettings& settings, const RenderVolumeCollection* rvc) const
{
	/*
	 * Always execure reflective shadow mapping indirect light render pass
	*/

	return true;
}

std::string HybridRSMIndirectDiffuseLightRenderPass::GetPostProcessFragmentShaderPath () const
{
	return "Assets/Shaders/HybridGlobalIllumination/hybridRSMIndirectDiffuseFragment.glsl";
}

std::string HybridRSMIndirectDiffuseLightRenderPass::GetPostProcessVolumeName () const
{
	return "HybridRSMIndirectDiffuseMap";
}

glm::ivec2 HybridRSMIndirectDiffuseLightRenderPass::GetPostProcessVolumeResolution (const RenderSettings& settings) const
{
	return glm::ivec2 (settings.resolution.width, settings.resolution.height);
}

FramebufferRenderVolume* HybridRSMIndirectDiffuseLightRenderPass::CreatePostProcessVolume (const RenderSettings& settings) const
{
	/*
	 * Create reflective shadow mapping indirect diffuse light framebuffer
	*/

	Resource<Texture> texture = Resource<Texture> (new Texture ("rsmIndirectDiffuseMap"));

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

	auto hgiStatisticsObject = StatisticsManager::Instance ()->GetStatisticsObject <HGIStatisticsObject> ();

	hgiStatisticsObject->hgiRSMIndirectDiffuseMapVolume = renderVolume;

	return renderVolume;
}

std::vector<PipelineAttribute> HybridRSMIndirectDiffuseLightRenderPass::GetCustomAttributes (const Camera* camera,
	const RenderSettings& settings, RenderVolumeCollection* rvc)
{
	/*
	 * Attach post process volume attributes to pipeline
	*/

	std::vector<PipelineAttribute> attributes = PostProcessRenderPass::GetCustomAttributes (camera, settings, rvc);

	/*
	 * Attach screen space ambient occlusion attributes to pipeline
	*/

	PipelineAttribute rsmRadius;
	PipelineAttribute ssdoRadius;
	PipelineAttribute hgiIntensity;
	PipelineAttribute temporalFilterEnabled;
	PipelineAttribute hgiInterpolationEnabled;
	PipelineAttribute hgiInterpolationScale;
	PipelineAttribute hgiMinInterpolationDistance;
	PipelineAttribute hgiMinInterpolationAngle;
	PipelineAttribute hgiDebugInterpolation;

	rsmRadius.type = PipelineAttribute::AttrType::ATTR_1F;
	ssdoRadius.type = PipelineAttribute::AttrType::ATTR_1F;
	hgiIntensity.type = PipelineAttribute::AttrType::ATTR_1F;
	temporalFilterEnabled.type = PipelineAttribute::AttrType::ATTR_1I;
	hgiInterpolationEnabled.type = PipelineAttribute::AttrType::ATTR_1I;
	hgiInterpolationScale.type = PipelineAttribute::AttrType::ATTR_1F;
	hgiMinInterpolationDistance.type = PipelineAttribute::AttrType::ATTR_1F;
	hgiMinInterpolationAngle.type = PipelineAttribute::AttrType::ATTR_1F;
	hgiDebugInterpolation.type = PipelineAttribute::AttrType::ATTR_1I;

	rsmRadius.name = "rsmRadius";
	ssdoRadius.name = "ssdoRadius";
	hgiIntensity.name = "hgiIntensity";
	temporalFilterEnabled.name = "temporalFilterEnabled";
	hgiInterpolationEnabled.name = "hgiInterpolationEnabled";
	hgiInterpolationScale.name = "hgiInterpolationScale";
	hgiMinInterpolationDistance.name = "hgiMinInterpolationDistance";
	hgiMinInterpolationAngle.name = "hgiMinInterpolationAngle";
	hgiDebugInterpolation.name = "hgiDebugInterpolation";

	rsmRadius.value.x = settings.hgi_rsm_radius;
	ssdoRadius.value.x = settings.hgi_ssdo_radius;
	hgiIntensity.value.x = settings.hgi_rsm_indirect_diffuse_intensity;
	temporalFilterEnabled.value.x = settings.hgi_temporal_filter_enabled;
	hgiInterpolationEnabled.value.x = settings.hgi_interpolation_enabled;
	hgiInterpolationScale.value.x = settings.hgi_interpolation_scale;
	hgiMinInterpolationDistance.value.x = settings.hgi_min_interpolation_distance;
	hgiMinInterpolationAngle.value.x = std::cos (DEG2RAD * settings.hgi_min_interpolation_angle);
	hgiDebugInterpolation.value.x = settings.hgi_debug_interpolation;

	attributes.push_back (rsmRadius);
	attributes.push_back (ssdoRadius);
	attributes.push_back (hgiIntensity);
	attributes.push_back (temporalFilterEnabled);
	attributes.push_back (hgiInterpolationEnabled);
	attributes.push_back (hgiInterpolationScale);
	attributes.push_back (hgiMinInterpolationDistance);
	attributes.push_back (hgiMinInterpolationAngle);
	attributes.push_back (hgiDebugInterpolation);

	return attributes;
}
