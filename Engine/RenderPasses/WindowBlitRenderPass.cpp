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

	FrameBuffer2DVolume* frameBuffer = (FrameBuffer2DVolume*) rvc->GetRenderVolume ("LightAccumulationVolume");

	/*
	* Render skybox
	*/

	EndDrawing (frameBuffer, settings);

	return rvc;
}

void WindowBlitRenderPass::Clear ()
{
	/*
	 * Nothing
	*/
}

void WindowBlitRenderPass::EndDrawing (FrameBuffer2DVolume* frameBufferVolume, const RenderSettings& settings)
{
	/*
	 * Bind framebuffer to blit
	*/

	GL::BindFramebuffer (GL_DRAW_FRAMEBUFFER, 0);

	frameBufferVolume->BindForBliting ();

	GL::BlitFramebuffer (settings.viewport.x, settings.viewport.y,
		settings.viewport.width, settings.viewport.height,
		settings.viewport.x, settings.viewport.y,
		settings.viewport.width, settings.viewport.height,
		GL_COLOR_BUFFER_BIT, GL_NEAREST);
}