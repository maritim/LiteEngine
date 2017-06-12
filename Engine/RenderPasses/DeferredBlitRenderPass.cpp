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

RenderVolumeCollection* DeferredBlitRenderPass::Execute (Scene* scene, Camera* camera, RenderVolumeCollection* rvc)
{
	/*
	 * Get frame buffer from render volume collection
	*/

	GBuffer* frameBuffer = (GBuffer*)rvc->GetRenderVolume ("GBuffer");

	/*
	* Render skybox
	*/

	EndDrawing (frameBuffer);

	return rvc;
}

void DeferredBlitRenderPass::EndDrawing (GBuffer* frameBuffer)
{
	frameBuffer->BindForFinalPass ();

	std::size_t windowWidth = Window::GetWidth ();
	std::size_t windowHeight = Window::GetHeight ();

	GL::BlitFramebuffer (0, 0, windowWidth, windowHeight, 0, 0, windowWidth, windowHeight,
		GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT, GL_NEAREST);
}