#include "HybridSSDOSamplesGenerationRenderPass.h"

#include "Core/Console/Console.h"

HybridSSDOSamplesGenerationRenderPass::HybridSSDOSamplesGenerationRenderPass () :
	_hgiSamplesVolume (nullptr)
{

}

void HybridSSDOSamplesGenerationRenderPass::Init (const RenderSettings& settings)
{
	/*
	 * Create screen space directional occlusion samples volume
	*/

	_hgiSamplesVolume = new HGISamplesVolume (settings.hgi_ssdo_samples);
}

RenderVolumeCollection* HybridSSDOSamplesGenerationRenderPass::Execute (const RenderScene* renderScene, const Camera* camera,
	const RenderSettings& settings, RenderVolumeCollection* rvc)
{
	/*
	 * Update screen space directional occlusion samples volume
	*/

	UpdateSamplesVolume (settings);

	return rvc->Insert ("HybridSSDOSamplesVolume", _hgiSamplesVolume);
}

bool HybridSSDOSamplesGenerationRenderPass::IsAvailable (const RenderScene* renderScenee, const Camera* camera,
	const RenderSettings& settings, const RenderVolumeCollection* rvc) const
{
	/*
	 * Always execute screen space directional occlusion samples generation render pass
	*/

	return true;
}

void HybridSSDOSamplesGenerationRenderPass::Clear ()
{
	/*
	 * Clear screen space directional occlusion samples volume
	*/

	delete _hgiSamplesVolume;
}

void HybridSSDOSamplesGenerationRenderPass::UpdateSamplesVolume (const RenderSettings& settings)
{
	if (_hgiSamplesVolume->GetSamplesCount () != settings.hgi_ssdo_samples) {

		/*
		 * Clear screen space ambient occlusion samples volume
		*/

		delete _hgiSamplesVolume;

		/*
		 * Create screen space ambient occlusion samples volume
		*/

		_hgiSamplesVolume = new HGISamplesVolume (settings.hgi_ssdo_samples);
	}
}
