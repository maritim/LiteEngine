#include "TRSMBlurRenderPass.h"

#include "TRSMIndirectDiffuseLightMapVolume.h"

#include "Debug/Statistics/StatisticsManager.h"
#include "TRSMStatisticsObject.h"

bool TRSMBlurRenderPass::IsAvailable (const RenderScene* renderScene, const Camera* camera,
	const RenderSettings& settings, const RenderVolumeCollection* rvc) const
{
	/*
	 * Check if screen space ambient occlusion blur is enabled
	*/

	return settings.trsm_blur_enabled;
}

std::string TRSMBlurRenderPass::GetPostProcessFragmentShaderPath () const
{
	return "Assets/Shaders/ReflectiveShadowMapping/reflectiveShadowMapBlurFragment.glsl";
}

std::string TRSMBlurRenderPass::GetPostProcessVolumeName () const
{
	return "IndirectDiffuseMap";
}

glm::ivec2 TRSMBlurRenderPass::GetPostProcessVolumeResolution (const RenderSettings& settings) const
{
	if (_postProcessMapVolume != nullptr) {
	}

	return glm::ivec2 (glm::vec2 (settings.resolution.width, settings.resolution.height) * settings.rsm_scale);
}

FramebufferRenderVolume* TRSMBlurRenderPass::CreatePostProcessVolume (const RenderSettings& settings) const
{
	/*
	 * Create temporal reflective shadow mapping blur framebuffer
	*/

	Resource<Texture> texture = Resource<Texture> (new Texture (""));

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

	FramebufferRenderVolume* renderVolume = new TRSMIndirectDiffuseLightMapVolume (framebuffer);

	/*
	 * Update statistics object
	*/

	auto trsmStatisticsObject = StatisticsManager::Instance ()->GetStatisticsObject <TRSMStatisticsObject> ();

	trsmStatisticsObject->trsmBlurMapVolume = renderVolume;

	return renderVolume;
}

std::vector<PipelineAttribute> TRSMBlurRenderPass::GetCustomAttributes (const Camera* camera,
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

	rsmResolution.type = PipelineAttribute::AttrType::ATTR_2F;

	rsmResolution.name = "rsmResolution";

	glm::ivec2 resolution = glm::ivec2 (glm::vec2 (settings.resolution.width, settings.resolution.height) * settings.rsm_scale);

	rsmResolution.value = glm::vec3 (resolution, 0.0f);

	attributes.push_back (rsmResolution);

	return attributes;
}
