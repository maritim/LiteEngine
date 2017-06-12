#include "DeferredSkyboxRenderPass.h"

#include "Renderer/Pipeline.h"

#include "Wrappers/OpenGL/GL.h"

DeferredSkyboxRenderPass::~DeferredSkyboxRenderPass ()
{

}

void DeferredSkyboxRenderPass::Init ()
{

}

RenderVolumeCollection* DeferredSkyboxRenderPass::Execute (Scene* scene, Camera* camera, RenderVolumeCollection* rvc)
{
	/*
	 * Render skybox
	*/

	SkyboxPass (camera);

	return rvc;
}

void DeferredSkyboxRenderPass::SkyboxPass (Camera* camera)
{
	GL::Enable (GL_STENCIL_TEST);
	GL::StencilFunc (GL_EQUAL, 0, 0xFF);
	GL::StencilOp (GL_KEEP, GL_KEEP, GL_KEEP);

	Pipeline::CreateProjection (camera->GetProjectionMatrix ());
	Pipeline::SendCamera (camera);

	Skybox::Render ();

	GL::Disable (GL_STENCIL_TEST);
}
