#include "TRSMSamplesGenerationRenderPass.h"

#include "TRSMSamplesVolume.h"

void TRSMSamplesGenerationRenderPass::Init (const RenderSettings& settings)
{
	/*
	 * Initialize reflective shadow map samples volume
	*/

	_rsmSamplesVolume = new TRSMSamplesVolume (settings.rsm_samples);
}

void TRSMSamplesGenerationRenderPass::UpdateRSMSamplesVolume (const RenderSettings& settings)
{
	std::size_t rsmSamplesSize = _rsmSamplesVolume->GetSize ();

	if (rsmSamplesSize != settings.rsm_samples) {

		/*
		 * Clear reflective shadow map samples
		*/

		delete _rsmSamplesVolume;

		/*
		 * Initialize reflective shadow map samples
		*/

		_rsmSamplesVolume = new TRSMSamplesVolume (settings.rsm_samples);
	}
}
