#include "RenderModule.h"

RenderModule::RenderModule () :
	_rvc (nullptr)
{

}

RenderModule::~RenderModule ()
{

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

	/*
	 * Initialize render volume collection
	*/

	_rvc = new RenderVolumeCollection ();
}

void RenderModule::RenderScene (const Scene* scene, const Camera* camera)
{
	/*
	 * Iterate on every pass on associated order to draw scene
	*/

	for (RenderPassI* renderPass : _renderPasses) {
		_rvc = renderPass->Execute (scene, camera, _rvc);
	}
}

void RenderModule::ClearModule ()
{
	/*
	 * Delete render volume collection
	*/

	delete _rvc;

	/*
	 * Clear every render pass
	*/

	for (RenderPassI* renderPass : _renderPasses) {
		renderPass->Clear ();
	}

	/*
	 * Delete render passes
	*/

	for (RenderPassI* renderPass : _renderPasses) {
		delete renderPass;
	}

	_renderPasses.clear ();
}
