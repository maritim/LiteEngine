#include "RSMIndirectSpecularLightRenderPass.h"

#include "Debug/Statistics/StatisticsManager.h"
#include "RSMStatisticsObject.h"

bool RSMIndirectSpecularLightRenderPass::IsAvailable (const RenderScene* renderScene, const Camera* camera,
	const RenderSettings& settings, const RenderVolumeCollection* rvc) const
{
	/*
	 * Check reflective shadow mapping indirect specular is enabled
	*/

	return settings.indirect_specular_enabled;
}

std::string RSMIndirectSpecularLightRenderPass::GetPostProcessFragmentShaderPath () const
{
	return "Assets/Shaders/ReflectiveShadowMapping/reflectiveShadowMapIndirectSpecularFragment.glsl";
}

std::string RSMIndirectSpecularLightRenderPass::GetPostProcessVolumeName () const
{
	return "IndirectSpecularMap";
}

glm::ivec2 RSMIndirectSpecularLightRenderPass::GetPostProcessVolumeResolution (const RenderSettings& settings) const
{
	return glm::ivec2 (glm::vec2 (settings.resolution.width, settings.resolution.height) * settings.rsm_scale);
}

FramebufferRenderVolume* RSMIndirectSpecularLightRenderPass::CreatePostProcessVolume (const RenderSettings& settings) const
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

	auto rsmStatisticsObject = StatisticsManager::Instance ()->GetStatisticsObject <RSMStatisticsObject> ();

	rsmStatisticsObject->rsmIndirectSpecularMapVolume = renderVolume;

	return renderVolume;
}

std::vector<PipelineAttribute> RSMIndirectSpecularLightRenderPass::GetCustomAttributes (const Camera* camera,
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
	PipelineAttribute rsmIterations;
	PipelineAttribute rsmThickness;
	PipelineAttribute rsmSpecularIntensity;

	rsmResolution.type = PipelineAttribute::AttrType::ATTR_2F;
	rsmIterations.type = PipelineAttribute::AttrType::ATTR_1I;
	rsmThickness.type = PipelineAttribute::AttrType::ATTR_1F;
	rsmSpecularIntensity.type = PipelineAttribute::AttrType::ATTR_1F;

	rsmResolution.name = "rsmResolution";
	rsmIterations.name = "rsmIterations";
	rsmThickness.name = "rsmThickness";
	rsmSpecularIntensity.name = "rsmSpecularIntensity";

	glm::ivec2 resolution = glm::ivec2 (glm::vec2 (settings.resolution.width, settings.resolution.height) * settings.rsm_scale);

	rsmResolution.value = glm::vec3 (resolution, 0.0f);
	rsmIterations.value.x = settings.rsm_iterations;
	rsmThickness.value.x = settings.rsm_thickness;
	rsmSpecularIntensity.value.x = settings.rsm_indirect_specular_intensity;

	attributes.push_back (rsmResolution);
	attributes.push_back (rsmIterations);
	attributes.push_back (rsmThickness);
	attributes.push_back (rsmSpecularIntensity);

	return attributes;
}
