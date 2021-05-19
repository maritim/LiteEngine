#include "RSMInterpolatedIndirectDiffuseLightRenderPass.h"

#include "Debug/Statistics/StatisticsManager.h"
#include "RSMStatisticsObject.h"

bool RSMInterpolatedIndirectDiffuseLightRenderPass::IsAvailable (const RenderScene* renderScene, const Camera* camera,
	const RenderSettings& settings, const RenderVolumeCollection* rvc) const
{
	/*
	 * Always execure reflective shadow mapping indirect light render pass
	*/

	return true;
}

std::string RSMInterpolatedIndirectDiffuseLightRenderPass::GetPostProcessFragmentShaderPath () const
{
	return "Assets/Shaders/ReflectiveShadowMapping/reflectiveShadowMapInterpolatedIndirectDiffuseFragment.glsl";
}

std::string RSMInterpolatedIndirectDiffuseLightRenderPass::GetPostProcessVolumeName () const
{
	return "IndirectDiffuseMap";
}

glm::ivec2 RSMInterpolatedIndirectDiffuseLightRenderPass::GetPostProcessVolumeResolution (const RenderSettings& settings) const
{
	return glm::ivec2 (glm::vec2 (settings.resolution.width, settings.resolution.height) * settings.rsm_interpolation_scale);
}

FramebufferRenderVolume* RSMInterpolatedIndirectDiffuseLightRenderPass::CreatePostProcessVolume (const RenderSettings& settings) const
{
	/*
	 * Create rsm interpolated indirect diffuse light framebuffer
	*/

	Resource<Texture> texture = Resource<Texture> (new Texture ("indirectDiffuseMap"));

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

	auto rsmStatisticsObject = StatisticsManager::Instance ()->GetStatisticsObject <RSMStatisticsObject> ();

	rsmStatisticsObject->rsmInterpolatedIndirectDiffuseMapVolume = renderVolume;

	return renderVolume;
}

std::vector<PipelineAttribute> RSMInterpolatedIndirectDiffuseLightRenderPass::GetCustomAttributes (const Camera* camera,
	const RenderSettings& settings, RenderVolumeCollection* rvc)
{
	/*
	 * Attach post process volume attributes to pipeline
	*/

	std::vector<PipelineAttribute> attributes = PostProcessRenderPass::GetCustomAttributes (camera, settings, rvc);

	/*
	 * Attach screen space ambient occlusion attributes to pipeline
	*/

	PipelineAttribute rsmResolution;
	PipelineAttribute rsmRadius;
	PipelineAttribute rsmIntensity;
	PipelineAttribute rsmInterpolationScale;

	rsmResolution.type = PipelineAttribute::AttrType::ATTR_2F;
	rsmRadius.type = PipelineAttribute::AttrType::ATTR_1F;
	rsmIntensity.type = PipelineAttribute::AttrType::ATTR_1F;
	rsmInterpolationScale.type = PipelineAttribute::AttrType::ATTR_1F;

	rsmResolution.name = "rsmResolution";
	rsmRadius.name = "rsmRadius";
	rsmIntensity.name = "rsmIntensity";
	rsmInterpolationScale.name = "rsmInterpolationScale";

	glm::ivec2 resolution = glm::ivec2 (glm::vec2 (settings.resolution.width, settings.resolution.height) * settings.rsm_interpolation_scale);

	rsmResolution.value = glm::vec3 (resolution, 0.0f);
	rsmRadius.value.x = settings.rsm_radius;
	rsmIntensity.value.x = settings.rsm_indirect_diffuse_intensity;
	rsmInterpolationScale.value.x = settings.rsm_interpolation_scale;

	attributes.push_back (rsmResolution);
	attributes.push_back (rsmRadius);
	attributes.push_back (rsmIntensity);
	attributes.push_back (rsmInterpolationScale);

	return attributes;
}
