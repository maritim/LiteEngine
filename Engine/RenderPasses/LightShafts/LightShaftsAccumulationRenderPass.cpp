#include "LightShaftsAccumulationRenderPass.h"

bool LightShaftsAccumulationRenderPass::IsAvailable (const RenderScene* renderScene, const Camera* camera,
	const RenderSettings& settings, const RenderVolumeCollection* rvc) const
{
	/*
	 * Check if bloom is enabled
	*/

	return settings.light_shafts_enabled;
}

std::string LightShaftsAccumulationRenderPass::GetPostProcessFragmentShaderPath () const
{
	return "Assets/Shaders/LightShafts/lightShaftsAccumulationFragment.glsl";
}

std::string LightShaftsAccumulationRenderPass::GetPostProcessVolumeName () const
{
	return "PostProcessMapVolume";
}

glm::ivec2 LightShaftsAccumulationRenderPass::GetPostProcessVolumeResolution (const RenderSettings& settings) const
{
	return glm::ivec2 (settings.resolution.width, settings.resolution.height);
}

FramebufferRenderVolume* LightShaftsAccumulationRenderPass::CreatePostProcessVolume (const RenderSettings& settings) const
{
	/*
	 * Create bloom accumulation framebuffer
	*/

	Resource<Texture> texture = Resource<Texture> (new Texture ("postProcessMap"));

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

	return new FramebufferRenderVolume (framebuffer);
}
