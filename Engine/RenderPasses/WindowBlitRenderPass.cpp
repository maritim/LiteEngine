#include "WindowBlitRenderPass.h"

#include "Renderer/Pipeline.h"

#include "Wrappers/OpenGL/GL.h"

WindowBlitRenderPass::~WindowBlitRenderPass ()
{

}

void WindowBlitRenderPass::Init (const RenderSettings& settings)
{

}

RenderVolumeCollection* WindowBlitRenderPass::Execute (const RenderScene* renderScene, const Camera* camera,
	const RenderSettings& settings, RenderVolumeCollection* rvc)
{
	/*
	 * Get depth buffer from render volume collection
	*/

	auto resultVolume = (FramebufferRenderVolume*) rvc->GetRenderVolume ("ResultFramebufferRenderVolume");

	/*
	* Render skybox
	*/

	EndDrawing (resultVolume, settings);

	return rvc;
}

void WindowBlitRenderPass::Clear ()
{
	/*
	 * Nothing
	*/
}

void WindowBlitRenderPass::EndDrawing (FramebufferRenderVolume* frameBufferVolume, const RenderSettings& settings)
{
	/*
	 * Bind framebuffer to blit
	*/

	GL::BindFramebuffer (GL_DRAW_FRAMEBUFFER, 0);

	frameBufferVolume->GetFramebufferView ()->ActivateSource ();

	GL::BlitFramebuffer (settings.viewport.x, settings.viewport.y,
		settings.viewport.width, settings.viewport.height,
		settings.viewport.x, settings.viewport.y,
		settings.viewport.width, settings.viewport.height,
		GL_COLOR_BUFFER_BIT, GL_NEAREST);
}