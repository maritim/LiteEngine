#include "IdleRenderPass.h"

#include "Wrappers/OpenGL/GL.h"

#include "Core/Console/Console.h"

IdleRenderPass::IdleRenderPass () :
	_framebufferRenderVolume (nullptr)
{

}

void IdleRenderPass::Init (const RenderSettings& settings)
{
	InitVolume (settings);
}

RenderVolumeCollection* IdleRenderPass::Execute (const RenderScene* renderScene, const Camera* camera,
	const RenderSettings& settings, RenderVolumeCollection* rvc)
{
	/*
	 * Update volume
	*/

	UpdateVolume (settings);

	/*
	 * Get frame buffer from render volume collection
	*/

	auto resultVolume = (FramebufferRenderVolume*) rvc->GetRenderVolume ("ResultFramebufferRenderVolume");

	_framebufferRenderVolume->GetFramebufferView ()->Activate ();
	resultVolume->GetFramebufferView ()->ActivateSource ();

	GL::BlitFramebuffer (settings.viewport.x, settings.viewport.y, 
		settings.viewport.width, settings.viewport.height,
		settings.viewport.x, settings.viewport.y,
		settings.viewport.width, settings.viewport.height,
		GL_COLOR_BUFFER_BIT, GL_NEAREST);

	return rvc->Insert ("PostProcessMapVolume", _framebufferRenderVolume);
}

bool IdleRenderPass::IsAvailable (const RenderScene* renderScene, const Camera* camera,
	const RenderSettings& settings, const RenderVolumeCollection* rvc) const
{
	/*
	 * Always execute idle container render sub pass
	*/

	return true;
}

void IdleRenderPass::Clear ()
{
	/*
	 * Clear post process map volume
	*/

	delete _framebufferRenderVolume;
}

void IdleRenderPass::UpdateVolume (const RenderSettings& settings)
{
	Resolution resolution = settings.resolution;

	auto size = _framebufferRenderVolume->GetFramebuffer ()->GetTexture (0)->GetSize ();

	if ((std::size_t) size.width != resolution.width || (std::size_t) size.height != resolution.height) {

		/*
		 * Clear post process volume
		*/

		delete _framebufferRenderVolume;

		/*
		 * Initialize post process volume
		*/

		InitVolume (settings);
	}
}

void IdleRenderPass::InitVolume (const RenderSettings& settings)
{
	/*
	 * Create idle framebuffer
	*/

	Resource<Texture> texture = Resource<Texture> (new Texture ("postProcessMap"));

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

	_framebufferRenderVolume = new FramebufferRenderVolume (framebuffer);
}
