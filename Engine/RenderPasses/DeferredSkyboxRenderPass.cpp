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

	SkyboxPass (scene, camera);

	return rvc;
}

void DeferredSkyboxRenderPass::SkyboxPass (Scene* scene, Camera* camera)
{
	GL::Enable (GL_STENCIL_TEST);
	GL::StencilFunc (GL_EQUAL, 0, 0xFF);
	GL::StencilOp (GL_KEEP, GL_KEEP, GL_KEEP);

	Pipeline::CreateProjection (camera->GetProjectionMatrix ());
	Pipeline::SendCamera (camera);

	/*
	 * Get scene's skybox
	*/

	Skybox* sceneSkybox = scene->GetSkybox ();

	/*
	 * Check if there is an actual skybox attached
	*/

	if (sceneSkybox != nullptr) {
		sceneSkybox->GetRenderer ()->Draw ();
	}

	GL::Disable (GL_STENCIL_TEST);
}
