#include "VCTSubsurfaceScatteringRenderPass.h"

#include "Debug/Statistics/StatisticsManager.h"
#include "VCTStatisticsObject.h"

bool VCTSubsurfaceScatteringRenderPass::IsAvailable (const RenderScene* renderScene, const Camera* camera,
	const RenderSettings& settings, const RenderVolumeCollection* rvc) const
{
	/*
	 * Always execure reflective shadow mapping indirect light render pass
	*/

	return settings.subsurface_scattering_enabled;
}

std::string VCTSubsurfaceScatteringRenderPass::GetPostProcessFragmentShaderPath () const
{
	return "Assets/Shaders/VoxelConeTracing/voxelConeTracingSubsurfaceScatteringFragment.glsl";
}

std::string VCTSubsurfaceScatteringRenderPass::GetPostProcessVolumeName () const
{
	return "VCTSSSMap";
}

glm::ivec2 VCTSubsurfaceScatteringRenderPass::GetPostProcessVolumeResolution (const RenderSettings& settings) const
{
	return glm::ivec2 (glm::vec2 (settings.resolution.width, settings.resolution.height) * settings.rsm_scale);
}

FramebufferRenderVolume* VCTSubsurfaceScatteringRenderPass::CreatePostProcessVolume (const RenderSettings& settings) const
{
	/*
	 * Create subsurface scattering framebuffer
	*/

	Resource<Texture> texture = Resource<Texture> (new Texture ("subsurfaceScatteringMap"));

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

	vctStatisticsObject->vctSubsurfaceScatteringMapVolume = renderVolume;

	return renderVolume;
}

std::vector<PipelineAttribute> VCTSubsurfaceScatteringRenderPass::GetCustomAttributes (const Camera* camera,
	const RenderSettings& settings, RenderVolumeCollection* rvc)
{
	/*
	 * Attach post process volume attributes to pipeline
	*/

	std::vector<PipelineAttribute> attributes = PostProcessRenderPass::GetCustomAttributes (camera, settings, rvc);

	/*
	 * Attach screen space ambient occlusion attributes to pipeline
	*/

	PipelineAttribute refractiveIndirectIntensity;
	PipelineAttribute refractiveConeRatio;
	PipelineAttribute refractiveConeDistance;

	refractiveIndirectIntensity.type = PipelineAttribute::AttrType::ATTR_1F;
	refractiveConeRatio.type = PipelineAttribute::AttrType::ATTR_1F;
	refractiveConeDistance.type = PipelineAttribute::AttrType::ATTR_1F;

	refractiveIndirectIntensity.name = "refractiveIndirectIntensity";
	refractiveConeRatio.name = "refractiveConeRatio";
	refractiveConeDistance.name = "refractiveConeDistance";

	refractiveIndirectIntensity.value.x = settings.vct_indirect_refractive_intensity;
	refractiveConeRatio.value.x = settings.vct_refractive_cone_ratio;
	refractiveConeDistance.value.x = settings.vct_refractive_cone_distance;

	attributes.push_back (refractiveIndirectIntensity);
	attributes.push_back (refractiveConeRatio);
	attributes.push_back (refractiveConeDistance);

	return attributes;
}
