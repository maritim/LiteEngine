#include "RenderModule.h"

RenderModule::RenderModule () :
	_rvc (nullptr)
{

}

RenderModule::~RenderModule ()
{

}

void RenderModule::InitModule (const RenderSettings& settings)
{
	/*
	 * Init specialized render module
	*/

	this->Init ();

	/*
	 * Initialize every render pass
	*/

	for (RenderPassI* renderPass : _renderPasses) {
		renderPass->Init (settings);
	}

	/*
	 * Initialize render volume collection
	*/

	_rvc = new RenderVolumeCollection ();
}

RenderProduct RenderModule::Render (const RenderScene* renderScene, const Camera* camera, const RenderSettings& settings)
{
	/*
	 * Create render product
	*/

	RenderProduct product;

	/*
	 * Iterate on every pass on associated order to draw scene
	*/

	for (RenderPassI* renderPass : _renderPasses) {
		_rvc = renderPass->Execute (renderScene, camera, settings, _rvc);
	}

	/*
	 * Initialize render product
	*/

	product.resultVolume = _rvc->GetRenderVolume ("LightAccumulationVolume");

	return product;
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
