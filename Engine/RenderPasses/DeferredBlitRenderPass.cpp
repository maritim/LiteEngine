#include "DeferredBlitRenderPass.h"

#include "Renderer/Pipeline.h"

#include "Systems/Window/Window.h"

#include "Wrappers/OpenGL/GL.h"

DeferredBlitRenderPass::~DeferredBlitRenderPass ()
{

}

void DeferredBlitRenderPass::Init ()
{

}

RenderVolumeCollection* DeferredBlitRenderPass::Execute (const Scene* scene, const Camera* camera, RenderVolumeCollection* rvc)
{
	/*
	 * Get frame buffer from render volume collection
	*/

	FrameBuffer2DVolume* frameBuffer = (FrameBuffer2DVolume*) rvc->GetPreviousVolume ();

	/*
	* Render skybox
	*/

	EndDrawing (frameBuffer);

	return rvc;
}

void DeferredBlitRenderPass::Clear ()
{
	/*
	 * Nothing
	*/
}

void DeferredBlitRenderPass::EndDrawing (FrameBuffer2DVolume* frameBufferVolume)
{
	/*
	 * Disable writting to framebuffer
	*/

	GL::BindFramebuffer (GL_DRAW_FRAMEBUFFER, 0);

	frameBufferVolume->BindForBliting ();

	std::size_t windowWidth = Window::GetWidth ();
	std::size_t windowHeight = Window::GetHeight ();

	GL::BlitFramebuffer (0, 0, windowWidth, windowHeight, 0, 0, windowWidth, windowHeight,
		GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT, GL_NEAREST);
}