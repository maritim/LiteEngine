#include "HybridSSDOInterpolatedIndirectDiffuseLightRenderPass.h"

#include "Debug/Statistics/StatisticsManager.h"
#include "HGIStatisticsObject.h"

bool HybridSSDOInterpolatedIndirectDiffuseLightRenderPass::IsAvailable (const RenderScene* renderScene, const Camera* camera,
	const RenderSettings& settings, const RenderVolumeCollection* rvc) const
{
	/*
	 * Always execure reflective shadow mapping indirect light render pass
	*/

	return true;
}

std::string HybridSSDOInterpolatedIndirectDiffuseLightRenderPass::GetPostProcessFragmentShaderPath () const
{
	return "Assets/Shaders/HybridGlobalIllumination/hybridSSDOInterpolatedIndirectDiffuseFragment.glsl";
}

std::string HybridSSDOInterpolatedIndirectDiffuseLightRenderPass::GetPostProcessVolumeName () const
{
	return "HybridSSDOIndirectDiffuseMap";
}

glm::ivec2 HybridSSDOInterpolatedIndirectDiffuseLightRenderPass::GetPostProcessVolumeResolution (const RenderSettings& settings) const
{
	return glm::ivec2 (glm::vec2 (settings.resolution.width, settings.resolution.height) * settings.hgi_interpolation_scale);
}

FramebufferRenderVolume* HybridSSDOInterpolatedIndirectDiffuseLightRenderPass::CreatePostProcessVolume (const RenderSettings& settings) const
{
	/*
	 * Create reflective shadow mapping indirect diffuse light framebuffer
	*/

	Resource<Texture> texture = Resource<Texture> (new Texture ("ssdoIndirectDiffuseMap"));

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

	auto hgiStatisticsObject = StatisticsManager::Instance ()->GetStatisticsObject <HGIStatisticsObject> ();

	hgiStatisticsObject->hgiSSDOInterpolatedIndirectDiffuseMapVolume = renderVolume;

	return renderVolume;
}

std::vector<PipelineAttribute> HybridSSDOInterpolatedIndirectDiffuseLightRenderPass::GetCustomAttributes (const Camera* camera,
	const RenderSettings& settings, RenderVolumeCollection* rvc)
{
	/*
	 * Attach post process volume attributes to pipeline
	*/

	std::vector<PipelineAttribute> attributes = PostProcessRenderPass::GetCustomAttributes (camera, settings, rvc);

	/*
	 * Attach screen space ambient occlusion attributes to pipeline
	*/

	PipelineAttribute hgiResolution;
	PipelineAttribute ssdoRadius;
	PipelineAttribute hgiIntensity;
	PipelineAttribute hgiInterpolationScale;

	hgiResolution.type = PipelineAttribute::AttrType::ATTR_2F;
	ssdoRadius.type = PipelineAttribute::AttrType::ATTR_1F;
	hgiIntensity.type = PipelineAttribute::AttrType::ATTR_1F;
	hgiInterpolationScale.type = PipelineAttribute::AttrType::ATTR_1F;

	hgiResolution.name = "hgiResolution";
	ssdoRadius.name = "ssdoRadius";
	hgiIntensity.name = "hgiIntensity";
	hgiInterpolationScale.name = "hgiInterpolationScale";

	hgiResolution.value = glm::vec3 (GetPostProcessVolumeResolution (settings), 0.0);
	ssdoRadius.value.x = settings.hgi_ssdo_radius;
	hgiIntensity.value.x = settings.hgi_ssdo_indirect_diffuse_intensity;
	hgiInterpolationScale.value.x = settings.hgi_interpolation_scale;

	attributes.push_back (hgiResolution);
	attributes.push_back (ssdoRadius);
	attributes.push_back (hgiIntensity);
	attributes.push_back (hgiInterpolationScale);

	return attributes;
}
