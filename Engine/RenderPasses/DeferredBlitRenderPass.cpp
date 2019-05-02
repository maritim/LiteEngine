#include "DeferredBlitRenderPass.h"

#include "Renderer/Pipeline.h"

#include "Wrappers/OpenGL/GL.h"

DeferredBlitRenderPass::~DeferredBlitRenderPass ()
{

}

void DeferredBlitRenderPass::Init (const RenderSettings& settings)
{

}

RenderVolumeCollection* DeferredBlitRenderPass::Execute (const Scene* scene, const Camera* camera,
	const RenderSettings& settings, RenderVolumeCollection* rvc)
{
	/*
	 * Get frame buffer from render volume collection
	*/

	FrameBuffer2DVolume* frameBuffer = (FrameBuffer2DVolume*) rvc->GetPreviousVolume ();

	/*
	 * Get depth buffer from render volume collection
	*/

	FrameBuffer2DVolume* resultFramebuffer = (FrameBuffer2DVolume*) rvc->GetRenderVolume ("LightAccumulationVolume");

	/*
	* Render skybox
	*/

	EndDrawing (frameBuffer, resultFramebuffer, settings);

	return rvc;
}

void DeferredBlitRenderPass::Clear ()
{
	/*
	 * Nothing
	*/
}

void DeferredBlitRenderPass::EndDrawing (FrameBuffer2DVolume* frameBufferVolume, FrameBuffer2DVolume* resultFramebuffer,
	const RenderSettings& settings)
{
	/*
	 * Bind framebuffer to blit
	*/

	frameBufferVolume->BindForBliting ();
	resultFramebuffer->BindToBlit ();

	GL::BlitFramebuffer (settings.viewport.x, settings.viewport.y,
		settings.viewport.width, settings.viewport.height,
		settings.viewport.x, settings.viewport.y,
		settings.viewport.width, settings.viewport.height,
		GL_COLOR_BUFFER_BIT, GL_NEAREST);
}