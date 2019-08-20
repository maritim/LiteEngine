#include "DeferredSkyboxRenderPass.h"

#include "Renderer/Pipeline.h"

#include "Wrappers/OpenGL/GL.h"

DeferredSkyboxRenderPass::~DeferredSkyboxRenderPass ()
{

}

void DeferredSkyboxRenderPass::Init (const RenderSettings& settings)
{

}

RenderVolumeCollection* DeferredSkyboxRenderPass::Execute (const RenderScene* renderScene, const Camera* camera,
	const RenderSettings& settings, RenderVolumeCollection* rvc)
{
	/*
	 * Start skybox pass
	*/

	StartSkyboxPass (rvc);

	/*
	 * Render skybox
	*/

	SkyboxPass (renderScene, camera, settings);

	return rvc;
}

void DeferredSkyboxRenderPass::Clear ()
{
	/*
	 * Nothing
	*/
}

void DeferredSkyboxRenderPass::StartSkyboxPass (RenderVolumeCollection* rvc)
{
	/*
	 * Bind light accumulation framebuffer for writing
	*/

	auto lightAccumulationVolume = rvc->GetRenderVolume ("LightAccumulationVolume");

	lightAccumulationVolume->BindForWriting ();
}

void DeferredSkyboxRenderPass::SkyboxPass (const RenderScene* renderScene, const Camera* camera, const RenderSettings& settings)
{
	/*
	 * Set viewport
	*/

	GL::Viewport (settings.viewport.x, settings.viewport.y,
		settings.viewport.width, settings.viewport.height);

	GL::Enable (GL_STENCIL_TEST);
	GL::StencilFunc (GL_EQUAL, 0, 0xFF);
	GL::StencilOp (GL_KEEP, GL_KEEP, GL_KEEP);

	GL::Enable (GL_DEPTH_TEST);
	GL::DepthMask (GL_FALSE);

	GL::Disable (GL_CULL_FACE);

	GL::BlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	Pipeline::CreateProjection (camera->GetProjectionMatrix ());
	Pipeline::SendCamera (camera);

	/*
	 * Get scene's skybox
	*/

	RenderSkyboxObject* renderSkyboxObject = renderScene->GetRenderSkyboxObject ();

	/*
	 * Check if there is an actual skybox attached
	*/

	if (renderSkyboxObject != nullptr) {
		renderSkyboxObject->Draw ();
	}

	GL::Disable (GL_STENCIL_TEST);
}
