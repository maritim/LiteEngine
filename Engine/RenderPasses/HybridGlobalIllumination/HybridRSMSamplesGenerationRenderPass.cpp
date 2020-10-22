#include "HybridRSMSamplesGenerationRenderPass.h"

#include "Core/Console/Console.h"

HybridRSMSamplesGenerationRenderPass::HybridRSMSamplesGenerationRenderPass () :
	_hgiSamplesVolume (nullptr)
{

}

void HybridRSMSamplesGenerationRenderPass::Init (const RenderSettings& settings)
{
	/*
	 * Create screen space directional occlusion samples volume
	*/

	_hgiSamplesVolume = new HGISamplesVolume (settings.hgi_rsm_samples);
}

RenderVolumeCollection* HybridRSMSamplesGenerationRenderPass::Execute (const RenderScene* renderScene, const Camera* camera,
	const RenderSettings& settings, RenderVolumeCollection* rvc)
{
	/*
	 * Update screen space directional occlusion samples volume
	*/

	UpdateSamplesVolume (settings);

	return rvc->Insert ("HybridRSMSamplesVolume", _hgiSamplesVolume);
}

bool HybridRSMSamplesGenerationRenderPass::IsAvailable (const RenderScene* renderScenee, const Camera* camera,
	const RenderSettings& settings, const RenderVolumeCollection* rvc) const
{
	/*
	 * Always execute screen space directional occlusion samples generation render pass
	*/

	return true;
}

void HybridRSMSamplesGenerationRenderPass::Clear ()
{
	/*
	 * Clear screen space directional occlusion samples volume
	*/

	delete _hgiSamplesVolume;
}

void HybridRSMSamplesGenerationRenderPass::UpdateSamplesVolume (const RenderSettings& settings)
{
	if (_hgiSamplesVolume->GetSamplesCount () != settings.hgi_rsm_samples) {

		/*
		 * Clear screen space ambient occlusion samples volume
		*/

		delete _hgiSamplesVolume;

		/*
		 * Create screen space ambient occlusion samples volume
		*/

		_hgiSamplesVolume = new HGISamplesVolume (settings.hgi_rsm_samples);
	}
}
