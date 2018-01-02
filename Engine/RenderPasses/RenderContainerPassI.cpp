#include "RenderContainerPassI.h"

RenderContainerPassI::RenderContainerPassI () :
	_renderSubPasses ()
{

}

RenderContainerPassI::~RenderContainerPassI ()
{
	/*
	 * Free memory of all sub passes
	*/

	for (auto renderSubPass : _renderSubPasses) {
		delete renderSubPass;
	}
}

void RenderContainerPassI::Init ()
{
	/*
	 * Container will be initialized by specialization
	*/

	InitContainer ();

	/*
	 * Iterate over every sub pass and initialize it
	*/

	for (auto renderSubPass : _renderSubPasses) {
		renderSubPass->Init ();
	}
}

RenderVolumeCollection* RenderContainerPassI::Execute (const Scene* scene, 
	const Camera* camera, RenderVolumeCollection* rvc)
{
	RenderVolumeI* volume = nullptr;

	/*
	 * Iterate every render volume provided by specialization
	*/

	while ((volume = NextVolume ()) != nullptr) {

		/*
		 * Execute all sub passes using provided volume
		*/

		IterateOverSubPasses (volume, scene, camera, rvc);
	}

	return rvc;
}

RenderVolumeCollection* RenderContainerPassI::IterateOverSubPasses (
	RenderVolumeI* volume, const Scene* scene, const Camera* camera,
	RenderVolumeCollection* rvc)
{
	/*
	 * Insert volume in render volume collection
	 * In this way it could be obtain at execution step
	*/

	rvc->Insert ("SubpassVolume", volume);

	/*
	 * Iterate all sub passes
	*/

	for (auto renderSubPass : _renderSubPasses) {

		/*
		 * Check if sub pass admits current volume
		*/

		if (!renderSubPass->IsAvailable (scene, camera, rvc)) {
			continue;
		}

		/*
		 * Execute render sub pass
		*/

		rvc = renderSubPass->Execute (scene, camera, rvc);
	}

	return rvc;
}
