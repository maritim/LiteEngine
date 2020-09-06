#include "HorizontalGaussianBlurRenderPass.h"

std::string HorizontalGaussianBlurRenderPass::GetPostProcessFragmentShaderPath () const
{
	return "Assets/Shaders/Blur/horizontalGaussianBlurFragment.glsl";
}

std::string HorizontalGaussianBlurRenderPass::GetPostProcessVolumeName () const
{
	return "BlurMapVolume";
}

FramebufferRenderVolume* HorizontalGaussianBlurRenderPass::CreatePostProcessVolume (const RenderSettings& settings) const
{
	Resource<Texture> texture = Resource<Texture> (new Texture ("blurMap"));

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

	return new FramebufferRenderVolume (framebuffer);
}

std::vector<PipelineAttribute> HorizontalGaussianBlurRenderPass::GetCustomAttributes (const Camera* camera,
	const RenderSettings& settings, RenderVolumeCollection* rvc)
{
	/*
	 * Attach post process volume attributes to pipeline
	*/

	std::vector<PipelineAttribute> attributes = PostProcessRenderPass::GetCustomAttributes (camera, settings, rvc);

	/*
	 * Attach horizontal gaussian blur attributes to pipeline
	*/

	PipelineAttribute blurMapResolution;

	blurMapResolution.type = PipelineAttribute::AttrType::ATTR_2F;

	blurMapResolution.name = "blurMapResolution";

	blurMapResolution.value = glm::vec3 (GetPostProcessVolumeResolution (settings), 0.0f);

	attributes.push_back (blurMapResolution);

	return attributes;
}
