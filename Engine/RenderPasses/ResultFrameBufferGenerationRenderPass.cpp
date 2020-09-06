#include "ResultFrameBufferGenerationRenderPass.h"

#include "Wrappers/OpenGL/GL.h"

#include "Core/Console/Console.h"

ResultFrameBufferGenerationRenderPass::ResultFrameBufferGenerationRenderPass () :
	_resultVolume (nullptr)
{

}

void ResultFrameBufferGenerationRenderPass::Init (const RenderSettings& settings)
{
	InitVolume (settings);
}

RenderVolumeCollection* ResultFrameBufferGenerationRenderPass::Execute (const RenderScene* renderScene, const Camera* camera,
	const RenderSettings& settings, RenderVolumeCollection* rvc)
{
	/*
	 * Update volume
	*/

	UpdateVolume (settings);

	/*
	 * Clear light accumulation buffer
	*/

	_resultVolume->GetFramebufferView ()->Activate ();

	GL::ClearColor (0, 0, 0, 0);
	GL::Clear (GL_COLOR_BUFFER_BIT);

	return rvc->Insert ("ResultFramebufferRenderVolume", _resultVolume);
}

bool ResultFrameBufferGenerationRenderPass::IsAvailable (const RenderScene* renderScene, const Camera* camera,
	const RenderSettings& settings, const RenderVolumeCollection* rvc) const
{
	/*
	 * Always execute idle container render sub pass
	*/

	return true;
}

void ResultFrameBufferGenerationRenderPass::Clear ()
{
	/*
	 * Clear post process map volume
	*/

	delete _resultVolume;
}

void ResultFrameBufferGenerationRenderPass::UpdateVolume (const RenderSettings& settings)
{
	Resolution resolution = settings.resolution;

	auto size = _resultVolume->GetFramebuffer ()->GetTexture (0)->GetSize ();

	if (size.width != resolution.width || size.height != resolution.height) {

		/*
		 * Clear post process volume
		*/

		delete _resultVolume;

		/*
		 * Initialize post process volume
		*/

		InitVolume (settings);
	}
}

void ResultFrameBufferGenerationRenderPass::InitVolume (const RenderSettings& settings)
{
	Resource<Texture> texture = Resource<Texture> (new Texture ("lightAccumulationMap"));

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

	_resultVolume = new FramebufferRenderVolume (framebuffer);
}