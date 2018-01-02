#include "ForwardRenderPass.h"

#include <algorithm>

#include "Wrappers/OpenGL/GL.h"

ForwardRenderPass::~ForwardRenderPass ()
{

}

void ForwardRenderPass::Init ()
{

}

RenderVolumeCollection* ForwardRenderPass::Execute (const Scene* scene, const Camera* camera, RenderVolumeCollection* rvc)
{
	/*
	* Forward Rendering Pass
	*/

	ForwardPass (scene);

	return rvc;
}

bool cmpForwardPass (Renderer* a, Renderer* b) {
	return a->GetPriority () < b->GetPriority ();
}

void ForwardRenderPass::ForwardPass (const Scene* scene)
{
	//TODO: Initialize camera projection here

	/*
	 * Enable depth test
	*/

	GL::Enable (GL_DEPTH_TEST);
	GL::DepthMask (GL_TRUE);

	/*
	* Render scene entities to framebuffer at Forward Rendering Stage
	*/

	std::vector<Renderer*> renderers;

	for (SceneObject* sceneObject : *scene) {
		if (sceneObject->GetRenderer ()->GetStageType () != Renderer::StageType::FORWARD_STAGE) {
			continue;
		}

		renderers.push_back (sceneObject->GetRenderer ());
	}

	std::sort (renderers.begin (), renderers.end (), cmpForwardPass);

	for (Renderer* renderer : renderers) {
		renderer->Draw ();
	}
}