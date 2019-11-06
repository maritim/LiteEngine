#include "ForwardRenderPass.h"

#include <algorithm>

#include "RenderPasses/FrameBuffer2DVolume.h"

#include "Wrappers/OpenGL/GL.h"

void ForwardRenderPass::Init (const RenderSettings& settings)
{

}

RenderVolumeCollection* ForwardRenderPass::Execute (const RenderScene* renderScene, const Camera* camera,
	const RenderSettings& settings, RenderVolumeCollection* rvc)
{
	/*
	 * Start forward pass
	*/

	StartForwardPass (rvc);

	/*
	* Forward Rendering Pass
	*/

	ForwardPass (renderScene);

	return rvc;
}

bool ForwardRenderPass::IsAvailable (const RenderScene* renderScene, const Camera* camera,
		const RenderSettings& settings, const RenderVolumeCollection* rvc) const
{
	/*
	 * Always execute skybox render sub pass
	*/

	return true;
}

void ForwardRenderPass::Clear ()
{
	/*
	 * Nothing
	*/
}

void ForwardRenderPass::StartForwardPass (RenderVolumeCollection* rvc)
{
	/*
	 * Bind framebuffer for writing
	*/

	FrameBuffer2DVolume* framebuffer = (FrameBuffer2DVolume*) rvc->GetRenderVolume ("ResultFrameBuffer2DVolume");
	framebuffer->BindForWriting ();
}

bool cmpForwardPass (RenderObject* a, RenderObject* b) {
	return a->GetPriority () < b->GetPriority ();
}

void ForwardRenderPass::ForwardPass (const RenderScene* renderScene)
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

	std::vector<RenderObject*> renderObjects;

	for_each_type (RenderObject*, renderObject, *renderScene) {

		/*
		 * Check if it's active
		*/

		if (renderObject->IsActive () == false) {
			continue;
		}
		
		if (renderObject->GetRenderStage () != RenderStage::RENDER_STAGE_FORWARD) {
			continue;
		}

		renderObjects.push_back (renderObject);
	}

	std::sort (renderObjects.begin (), renderObjects.end (), cmpForwardPass);

	for (RenderObject* renderObject : renderObjects) {
		renderObject->Draw ();
	}
}