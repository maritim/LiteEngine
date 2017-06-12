#include "RenderModule.h"

RenderModule::~RenderModule ()
{
	/*
	 * Delete render passes
	*/

	for (RenderPassI* renderPass : _renderPasses) {
		delete renderPass;
	}
}

void RenderModule::InitModule ()
{
	/*
	 * Init specialized render module
	*/

	this->Init ();

	/*
	 * Initialize every render pass
	*/

	for (RenderPassI* renderPass : _renderPasses) {
		renderPass->Init ();
	}
}

void RenderModule::RenderScene (Scene* scene, Camera* camera)
{
	/*
	 * Initialize a render volume collection
	*/

	static RenderVolumeCollection* rvc = new RenderVolumeCollection ();

	/*
	 * Iterate on every pass on associated order to draw scene
	*/

	for (RenderPassI* renderPass : _renderPasses) {
		rvc = renderPass->Execute (scene, camera, rvc);
	}
}
