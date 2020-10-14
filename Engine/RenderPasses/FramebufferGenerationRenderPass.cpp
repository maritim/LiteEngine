#include "FramebufferGenerationRenderPass.h"

#include "Wrappers/OpenGL/GL.h"

#include "Core/Console/Console.h"

FramebufferGenerationRenderPass::FramebufferGenerationRenderPass (const std::string& volumeName) :
	_volume (nullptr),
	_volumeName (volumeName)
{

}

void FramebufferGenerationRenderPass::Init (const RenderSettings& settings)
{
	InitVolume (settings);
}

RenderVolumeCollection* FramebufferGenerationRenderPass::Execute (const RenderScene* renderScene, const Camera* camera,
	const RenderSettings& settings, RenderVolumeCollection* rvc)
{
	/*
	 * Update volume
	*/

	UpdateVolume (settings);

	/*
	 * Clear light accumulation buffer
	*/

	_volume->GetFramebufferView ()->Activate ();

	GL::ClearColor (0, 0, 0, 0);
	GL::Clear (GL_COLOR_BUFFER_BIT);

	return rvc->Insert (_volumeName, _volume);
}

bool FramebufferGenerationRenderPass::IsAvailable (const RenderScene* renderScene, const Camera* camera,
	const RenderSettings& settings, const RenderVolumeCollection* rvc) const
{
	/*
	 * Always execute idle container render sub pass
	*/

	return true;
}

void FramebufferGenerationRenderPass::Clear ()
{
	/*
	 * Clear post process map volume
	*/

	delete _volume;
}

void FramebufferGenerationRenderPass::UpdateVolume (const RenderSettings& settings)
{
	Resolution resolution = settings.resolution;

	auto size = _volume->GetFramebuffer ()->GetTexture (0)->GetSize ();

	if (size.width != resolution.width || size.height != resolution.height) {

		/*
		 * Clear post process volume
		*/

		delete _volume;

		/*
		 * Initialize post process volume
		*/

		InitVolume (settings);
	}
}

void FramebufferGenerationRenderPass::InitVolume (const RenderSettings& settings)
{
	Resource<Texture> texture = Resource<Texture> (new Texture (_volumeName));

	texture->SetSize (Size (settings.resolution.width, settings.resolution.height));
	texture->SetMipmapGeneration (false);
	texture->SetSizedInternalFormat (TEXTURE_SIZED_INTERNAL_FORMAT::FORMAT_RGB16);
	texture->SetInternalFormat (TEXTURE_INTERNAL_FORMAT::FORMAT_RGB);
	texture->SetChannelType (TEXTURE_CHANNEL_TYPE::CHANNEL_FLOAT);
	texture->SetWrapMode (TEXTURE_WRAP_MODE::WRAP_CLAMP_EDGE);
	texture->SetMinFilter (TEXTURE_FILTER_MODE::FILTER_NEAREST);
	texture->SetMagFilter (TEXTURE_FILTER_MODE::FILTER_NEAREST);
	texture->SetAnisotropicFiltering (false);

	Resource<Framebuffer> framebuffer = Resource<Framebuffer> (new Framebuffer (texture));

	_volume = new FramebufferRenderVolume (framebuffer);
}
