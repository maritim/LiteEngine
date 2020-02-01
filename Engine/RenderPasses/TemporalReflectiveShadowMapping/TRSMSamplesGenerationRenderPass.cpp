#include "TRSMSamplesGenerationRenderPass.h"

#include "TRSMSamplesVolume.h"

TRSMSamplesGenerationRenderPass::TRSMSamplesGenerationRenderPass ()
{
	delete _reflectiveShadowMapSamplesVolume;

	_reflectiveShadowMapSamplesVolume = new TRSMSamplesVolume ();
}
