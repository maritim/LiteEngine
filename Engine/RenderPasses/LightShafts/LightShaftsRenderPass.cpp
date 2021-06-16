#include "LightShaftsRenderPass.h"

bool LightShaftsRenderPass::IsAvailable (const RenderScene* renderScene, const Camera* camera,
	const RenderSettings& settings, const RenderVolumeCollection* rvc) const
{
	/*
	 * Check if bloom is enabled
	*/

	return settings.light_shafts_enabled;
}

std::string LightShaftsRenderPass::GetPostProcessFragmentShaderPath () const
{
	return "Assets/Shaders/LightShafts/bloomAccumulationFragment.glsl";
}

std::string LightShaftsRenderPass::GetPostProcessVolumeName () const
{
	return "PostProcessMapVolume";
}

glm::ivec2 LightShaftsRenderPass::GetPostProcessVolumeResolution (const RenderSettings& settings) const
{
	return glm::ivec2 (settings.resolution.width, settings.resolution.height);
}

FramebufferRenderVolume* LightShaftsRenderPass::CreatePostProcessVolume (const RenderSettings& settings) const
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

std::vector<PipelineAttribute> LightShaftsRenderPass::GetCustomAttributes (const Camera* camera,
	const RenderSettings& settings, RenderVolumeCollection* rvc)
{
	/*
	 * Attach post process volume attributes to pipeline
	*/

	std::vector<PipelineAttribute> attributes = PostProcessRenderPass::GetCustomAttributes (camera, settings, rvc);

	/*
	 * Attach bloom attributes to pipeline
	*/

	// PipelineAttribute bloomIntensity;

	// bloomIntensity.type = PipelineAttribute::AttrType::ATTR_1F;

	// bloomIntensity.name = "bloomIntensity";

	// bloomIntensity.value.x = settings.bloom_intensity;

	// attributes.push_back (bloomIntensity);

	return attributes;
}
