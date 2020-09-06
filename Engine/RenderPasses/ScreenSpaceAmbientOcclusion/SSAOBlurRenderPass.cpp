#include "SSAOBlurRenderPass.h"

#include "Debug/Statistics/StatisticsManager.h"
#include "SSAOStatisticsObject.h"

bool SSAOBlurRenderPass::IsAvailable (const RenderScene* renderScene, const Camera* camera,
	const RenderSettings& settings, const RenderVolumeCollection* rvc) const
{
	/*
	 * Check if screen space ambient occlusion blur is enabled
	*/

	return settings.ssao_enabled && settings.ssao_blur_enabled;
}

std::string SSAOBlurRenderPass::GetPostProcessFragmentShaderPath () const
{
	return "Assets/Shaders/ScreenSpaceAmbientOcclusion/screenSpaceAmbientOcclusionBlurFragment.glsl";
}

std::string SSAOBlurRenderPass::GetPostProcessVolumeName () const
{
	return "SSAOMapVolume";
}

glm::ivec2 SSAOBlurRenderPass::GetPostProcessVolumeResolution (const RenderSettings& settings) const
{
	return glm::ivec2 (glm::vec2 (settings.resolution.width, settings.resolution.height) * settings.ssao_scale);
}

FramebufferRenderVolume* SSAOBlurRenderPass::CreatePostProcessVolume (const RenderSettings& settings) const
{
	/*
	 * Creature screen space ambient occlusion blur framebuffer
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

	auto ssaoStatisticsObject = StatisticsManager::Instance ()->GetStatisticsObject <SSAOStatisticsObject> ();

	ssaoStatisticsObject->ssaoMapVolume = renderVolume;

	return renderVolume;
}

std::vector<PipelineAttribute> SSAOBlurRenderPass::GetCustomAttributes (const Camera* camera,
	const RenderSettings& settings, RenderVolumeCollection* rvc)
{
	/*
	 * Attach post process volume attributes to pipeline
	*/

	std::vector<PipelineAttribute> attributes = PostProcessRenderPass::GetCustomAttributes (camera, settings, rvc);

	/*
	 * Attach screen space ambient occlusion attributes to pipeline
	*/

	PipelineAttribute ssaoResolution;

	ssaoResolution.type = PipelineAttribute::AttrType::ATTR_2F;

	ssaoResolution.name = "ssaoResolution";

	glm::ivec2 resolution = glm::ivec2 (glm::vec2 (settings.resolution.width, settings.resolution.height) * settings.ssao_scale);

	ssaoResolution.value = glm::vec3 (resolution, 0.0f);

	attributes.push_back (ssaoResolution);

	return attributes;
}
