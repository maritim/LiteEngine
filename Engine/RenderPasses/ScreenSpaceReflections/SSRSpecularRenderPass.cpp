#include "SSRSpecularRenderPass.h"

#include "Debug/Statistics/StatisticsManager.h"
#include "SSRStatisticsObject.h"

bool SSRSpecularRenderPass::IsAvailable (const RenderScene* renderScene, const Camera* camera,
	const RenderSettings& settings, const RenderVolumeCollection* rvc) const
{
	/*
	 * Check if screen space reflection is enabled
	*/

	return settings.ssr_enabled;
}

std::string SSRSpecularRenderPass::GetPostProcessFragmentShaderPath () const
{
	return "Assets/Shaders/ScreenSpaceReflections/screenSpaceReflectionSpecularFragment.glsl";
}

std::string SSRSpecularRenderPass::GetPostProcessVolumeName () const
{
	return "SSRSpecularMapVolume";
}

glm::ivec2 SSRSpecularRenderPass::GetPostProcessVolumeResolution (const RenderSettings& settings) const
{
	return glm::ivec2 (glm::vec2 (settings.resolution.width, settings.resolution.height) * settings.ssr_scale);
}

FramebufferRenderVolume* SSRSpecularRenderPass::CreatePostProcessVolume (const RenderSettings& settings) const
{
	/*
	 * Create screen space reflections framebuffer
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

	auto ssrStatisticsObject = StatisticsManager::Instance ()->GetStatisticsObject <SSRStatisticsObject> ();

	ssrStatisticsObject->ssrMapVolume = renderVolume;

	return renderVolume;
}

std::vector<PipelineAttribute> SSRSpecularRenderPass::GetCustomAttributes (const Camera* camera,
	const RenderSettings& settings, RenderVolumeCollection* rvc)
{
	/*
	 * Attach post process volume attributes to pipeline
	*/

	std::vector<PipelineAttribute> attributes = PostProcessRenderPass::GetCustomAttributes (camera, settings, rvc);

	/*
	 * Attach screen space ambient occlusion attributes to pipeline
	*/

	PipelineAttribute ssrIntensity;

	ssrIntensity.type = PipelineAttribute::AttrType::ATTR_1F;

	ssrIntensity.name = "ssrIntensity";

	ssrIntensity.value.x = settings.ssr_intensity;

	attributes.push_back (ssrIntensity);

	return attributes;
}
