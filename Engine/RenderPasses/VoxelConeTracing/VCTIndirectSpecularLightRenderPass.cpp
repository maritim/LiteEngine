#include "VCTIndirectSpecularLightRenderPass.h"

#include "Debug/Statistics/StatisticsManager.h"
#include "VCTStatisticsObject.h"

bool VCTIndirectSpecularLightRenderPass::IsAvailable (const RenderScene* renderScene, const Camera* camera,
	const RenderSettings& settings, const RenderVolumeCollection* rvc) const
{
	/*
	 * Always execure reflective shadow mapping indirect light render pass
	*/

	return true;
}

std::string VCTIndirectSpecularLightRenderPass::GetPostProcessFragmentShaderPath () const
{
	return "Assets/Shaders/VoxelConeTracing/voxelConeTracingIndirectSpecularFragment.glsl";
}

std::string VCTIndirectSpecularLightRenderPass::GetPostProcessVolumeName () const
{
	return "IndirectSpecularMap";
}

glm::ivec2 VCTIndirectSpecularLightRenderPass::GetPostProcessVolumeResolution (const RenderSettings& settings) const
{
	return glm::ivec2 (glm::vec2 (settings.resolution.width, settings.resolution.height) * settings.rsm_scale);
}

FramebufferRenderVolume* VCTIndirectSpecularLightRenderPass::CreatePostProcessVolume (const RenderSettings& settings) const
{
	/*
	 * Create indirect specular light framebuffer
	*/

	Resource<Texture> texture = Resource<Texture> (new Texture ("indirectSpecularMap"));

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

	vctStatisticsObject->vctIndirectSpecularMapVolume = renderVolume;

	return renderVolume;
}

std::vector<PipelineAttribute> VCTIndirectSpecularLightRenderPass::GetCustomAttributes (const Camera* camera,
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
	PipelineAttribute specularConeDistance;
	PipelineAttribute originBias;

	vctIndirectIntensity.type = PipelineAttribute::AttrType::ATTR_1F;
	specularConeDistance.type = PipelineAttribute::AttrType::ATTR_1F;
	originBias.type = PipelineAttribute::AttrType::ATTR_1F;

	vctIndirectIntensity.name = "vctIndirectIntensity";
	specularConeDistance.name = "specularConeDistance";
	originBias.name = "originBias";

	vctIndirectIntensity.value.x = settings.vct_indirect_specular_intensity;
	specularConeDistance.value.x = settings.vct_specular_cone_distance;
	originBias.value.x = settings.vct_origin_bias;

	attributes.push_back (vctIndirectIntensity);
	attributes.push_back (specularConeDistance);
	attributes.push_back (originBias);

	return attributes;
}
