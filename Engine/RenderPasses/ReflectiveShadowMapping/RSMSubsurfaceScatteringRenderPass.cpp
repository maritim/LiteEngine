#include "RSMSubsurfaceScatteringRenderPass.h"

#include "Debug/Statistics/StatisticsManager.h"
#include "RSMStatisticsObject.h"

bool RSMSubsurfaceScatteringRenderPass::IsAvailable (const RenderScene* renderScene, const Camera* camera,
	const RenderSettings& settings, const RenderVolumeCollection* rvc) const
{
	/*
	 * Check reflective shadow mapping subsurface scattering enabled
	*/

	return settings.subsurface_scattering_enabled;
}

std::string RSMSubsurfaceScatteringRenderPass::GetPostProcessFragmentShaderPath () const
{
	return "Assets/Shaders/ReflectiveShadowMapping/reflectiveShadowMapSubsurfaceScatteringFragment.glsl";
}

std::string RSMSubsurfaceScatteringRenderPass::GetPostProcessVolumeName () const
{
	return "SubsurfaceScatteringMap";
}

glm::ivec2 RSMSubsurfaceScatteringRenderPass::GetPostProcessVolumeResolution (const RenderSettings& settings) const
{
	return glm::ivec2 (settings.resolution.width, settings.resolution.height);
}

FramebufferRenderVolume* RSMSubsurfaceScatteringRenderPass::CreatePostProcessVolume (const RenderSettings& settings) const
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

	auto rsmStatisticsObject = StatisticsManager::Instance ()->GetStatisticsObject <RSMStatisticsObject> ();

	rsmStatisticsObject->rsmSubsurfaceScatteringMapVolume = renderVolume;

	return renderVolume;
}

std::vector<PipelineAttribute> RSMSubsurfaceScatteringRenderPass::GetCustomAttributes (const Camera* camera,
	const RenderSettings& settings, RenderVolumeCollection* rvc)
{
	/*
	 * Attach post process volume attributes to pipeline
	*/

	std::vector<PipelineAttribute> attributes = PostProcessRenderPass::GetCustomAttributes (camera, settings, rvc);

	/*
	 * Attach reflective shadow mapping subsurface scattering attributes to pipeline
	*/

	PipelineAttribute rsmResolution;
	PipelineAttribute rsmIterations;
	PipelineAttribute rsmThickness;
	PipelineAttribute rsmIndirectRefractiveIntensity;

	rsmResolution.type = PipelineAttribute::AttrType::ATTR_2F;
	rsmIterations.type = PipelineAttribute::AttrType::ATTR_1I;
	rsmThickness.type = PipelineAttribute::AttrType::ATTR_1F;
	rsmIndirectRefractiveIntensity.type = PipelineAttribute::AttrType::ATTR_1F;

	rsmResolution.name = "rsmResolution";
	rsmIterations.name = "rsmIterations";
	rsmThickness.name = "rsmThickness";
	rsmIndirectRefractiveIntensity.name = "rsmIndirectRefractiveIntensity";

	glm::ivec2 resolution = glm::ivec2 (glm::vec2 (settings.resolution.width, settings.resolution.height) * settings.rsm_scale);

	rsmResolution.value = glm::vec3 (resolution, 0.0f);
	rsmIterations.value.x = settings.rsm_iterations;
	rsmThickness.value.x = settings.rsm_thickness;
	rsmIndirectRefractiveIntensity.value.x = settings.rsm_indirect_refractive_intensity;

	attributes.push_back (rsmResolution);
	attributes.push_back (rsmIterations);
	attributes.push_back (rsmThickness);
	attributes.push_back (rsmIndirectRefractiveIntensity);

	return attributes;
}
