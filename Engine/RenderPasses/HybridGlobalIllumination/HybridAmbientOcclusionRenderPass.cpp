#include "HybridAmbientOcclusionRenderPass.h"

#include "Debug/Statistics/StatisticsManager.h"
#include "HGIStatisticsObject.h"

bool HybridAmbientOcclusionRenderPass::IsAvailable (const RenderScene* renderScene, const Camera* camera,
	const RenderSettings& settings, const RenderVolumeCollection* rvc) const
{
	/*
	 * Check if screen space ambient occlusion is enabled
	*/

	return settings.hgi_ao_enabled;
}

std::string HybridAmbientOcclusionRenderPass::GetPostProcessFragmentShaderPath () const
{
	return "Assets/Shaders/HybridGlobalIllumination/hybridAmbientOcclusionFragment.glsl";
}

std::string HybridAmbientOcclusionRenderPass::GetPostProcessVolumeName () const
{
	return "ambientOcclusionMap";
}

glm::ivec2 HybridAmbientOcclusionRenderPass::GetPostProcessVolumeResolution (const RenderSettings& settings) const
{
	return glm::ivec2 (settings.resolution.width, settings.resolution.height);
}

FramebufferRenderVolume* HybridAmbientOcclusionRenderPass::CreatePostProcessVolume (const RenderSettings& settings) const
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

	auto hgiStatisticsObject = StatisticsManager::Instance ()->GetStatisticsObject <HGIStatisticsObject> ();

	hgiStatisticsObject->hgiAmbientOcclusionMapVolume = renderVolume;

	return renderVolume;
}

std::vector<PipelineAttribute> HybridAmbientOcclusionRenderPass::GetCustomAttributes (const Camera* camera,
	const RenderSettings& settings, RenderVolumeCollection* rvc)
{
	/*
	 * Attach post process volume attributes to pipeline
	*/

	std::vector<PipelineAttribute> attributes = PostProcessRenderPass::GetCustomAttributes (camera, settings, rvc);

	/*
	 * Attach screen space ambient occlusion attributes to pipeline
	*/

	PipelineAttribute ssaoRadius;
	PipelineAttribute ssaoBias;
	PipelineAttribute hgiAOBlend;
	PipelineAttribute temporalFilterEnabled;

	ssaoRadius.type = PipelineAttribute::AttrType::ATTR_1F;
	ssaoBias.type = PipelineAttribute::AttrType::ATTR_1F;
	hgiAOBlend.type = PipelineAttribute::AttrType::ATTR_1F;
	temporalFilterEnabled.type = PipelineAttribute::AttrType::ATTR_1I;

	ssaoRadius.name = "ssaoRadius";
	ssaoBias.name = "ssaoBias";
	hgiAOBlend.name = "hgiAOBlend";
	temporalFilterEnabled.name = "temporalFilterEnabled";

	ssaoRadius.value.x = settings.hgi_ao_radius;
	ssaoBias.value.x = settings.hgi_ao_bias;
	hgiAOBlend.value.x = settings.hgi_ao_blend;
	temporalFilterEnabled.value.x = settings.hgi_temporal_filter_enabled;

	attributes.push_back (ssaoRadius);
	attributes.push_back (ssaoBias);
	attributes.push_back (hgiAOBlend);
	attributes.push_back (temporalFilterEnabled);

	return attributes;
}
