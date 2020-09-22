#include "VCTIndirectDiffuseLightRenderPass.h"

#include "Debug/Statistics/StatisticsManager.h"
#include "VCTStatisticsObject.h"

bool VCTIndirectDiffuseLightRenderPass::IsAvailable (const RenderScene* renderScene, const Camera* camera,
	const RenderSettings& settings, const RenderVolumeCollection* rvc) const
{
	/*
	 * Always execure reflective shadow mapping indirect light render pass
	*/

	return true;
}

std::string VCTIndirectDiffuseLightRenderPass::GetPostProcessFragmentShaderPath () const
{
	return "Assets/Shaders/VoxelConeTracing/voxelConeTracingIndirectDiffuseFragment.glsl";
}

std::string VCTIndirectDiffuseLightRenderPass::GetPostProcessVolumeName () const
{
	return "IndirectDiffuseMap";
}

glm::ivec2 VCTIndirectDiffuseLightRenderPass::GetPostProcessVolumeResolution (const RenderSettings& settings) const
{
	return glm::ivec2 (glm::vec2 (settings.resolution.width, settings.resolution.height) * settings.rsm_scale);
}

FramebufferRenderVolume* VCTIndirectDiffuseLightRenderPass::CreatePostProcessVolume (const RenderSettings& settings) const
{
	/*
	 * Create indirect diffuse light framebuffer
	*/

	Resource<Texture> texture = Resource<Texture> (new Texture ("indirectDiffuseMap"));

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

	auto vctStatisticsObject = StatisticsManager::Instance ()->GetStatisticsObject <VCTStatisticsObject> ();

	vctStatisticsObject->vctIndirectDiffuseMapVolume = renderVolume;

	return renderVolume;
}

std::vector<PipelineAttribute> VCTIndirectDiffuseLightRenderPass::GetCustomAttributes (const Camera* camera,
	const RenderSettings& settings, RenderVolumeCollection* rvc)
{
	/*
	 * Attach post process volume attributes to pipeline
	*/

	std::vector<PipelineAttribute> attributes = PostProcessRenderPass::GetCustomAttributes (camera, settings, rvc);

	/*
	 * Attach screen space ambient occlusion attributes to pipeline
	*/

	PipelineAttribute vctIndirectIntensity;
	PipelineAttribute diffuseConeDistance;
	PipelineAttribute diffuseOriginBias;

	vctIndirectIntensity.type = PipelineAttribute::AttrType::ATTR_1F;
	diffuseConeDistance.type = PipelineAttribute::AttrType::ATTR_1F;
	diffuseOriginBias.type = PipelineAttribute::AttrType::ATTR_1F;

	vctIndirectIntensity.name = "vctIndirectIntensity";
	diffuseConeDistance.name = "diffuseConeDistance";
	diffuseOriginBias.name = "diffuseOriginBias";

	vctIndirectIntensity.value.x = settings.vct_indirect_diffuse_intensity;
	diffuseConeDistance.value.x = settings.vct_diffuse_cone_distance;
	diffuseOriginBias.value.x = settings.vct_diffuse_origin_bias;

	attributes.push_back (vctIndirectIntensity);
	attributes.push_back (diffuseConeDistance);
	attributes.push_back (diffuseOriginBias);

	return attributes;
}
