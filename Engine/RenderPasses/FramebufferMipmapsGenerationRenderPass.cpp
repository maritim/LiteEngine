#include "FramebufferMipmapsGenerationRenderPass.h"

#include "Wrappers/OpenGL/GL.h"

FramebufferMipmapsGenerationRenderPass::FramebufferMipmapsGenerationRenderPass (const std::string& volumeName) :
	_volumeName (volumeName)
{

}

void FramebufferMipmapsGenerationRenderPass::Init (const RenderSettings& settings)
{
	/*
	 * Nothing
	*/
}

RenderVolumeCollection* FramebufferMipmapsGenerationRenderPass::Execute (const RenderScene* renderScene, const Camera* camera,
	const RenderSettings& settings, RenderVolumeCollection* rvc)
{
	/*
	 * Generate mipmaps
	*/

	GenerateMipmaps (rvc);

	return rvc;
}

bool FramebufferMipmapsGenerationRenderPass::IsAvailable (const RenderScene* renderScene, const Camera* camera,
		const RenderSettings& settings, const RenderVolumeCollection* rvc) const
{
	/*
	 * Always execute skybox render sub pass
	*/

	return true;
}

void FramebufferMipmapsGenerationRenderPass::Clear ()
{
	/*
	 * Nothing
	*/
}

void FramebufferMipmapsGenerationRenderPass::GenerateMipmaps (const RenderVolumeCollection* rvc) const
{
	auto framebuffer = (FramebufferRenderVolume*) (_volumeName != std::string () ?
		rvc->GetRenderVolume (_volumeName) :
		rvc->GetPreviousVolume ());

	/*
	 * Generate framebuffer mipmaps
	*/

	std::size_t textureCount = framebuffer->GetFramebufferView ()->GetTextureViewCount ();

	for (std::size_t index = 0 ; index < textureCount ; index++) {
		framebuffer->GetFramebufferView ()->GetTextureView (index)->Activate (0);

		GL::GenerateMipmap (GL_TEXTURE_2D);
	}
}
