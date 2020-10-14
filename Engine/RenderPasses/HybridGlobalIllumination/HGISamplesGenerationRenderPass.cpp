#include "HGISamplesGenerationRenderPass.h"

#include "Core/Console/Console.h"

HGISamplesGenerationRenderPass::HGISamplesGenerationRenderPass () :
	_hgiSamplesVolume (nullptr)
{

}

void HGISamplesGenerationRenderPass::Init (const RenderSettings& settings)
{
	/*
	 * Create screen space directional occlusion samples volume
	*/

	_hgiSamplesVolume = new HGISamplesVolume (settings.hgi_rsm_samples);
}

RenderVolumeCollection* HGISamplesGenerationRenderPass::Execute (const RenderScene* renderScene, const Camera* camera,
	const RenderSettings& settings, RenderVolumeCollection* rvc)
{
	/*
	 * Update screen space directional occlusion samples volume
	*/

	UpdateSamplesVolume (settings);

	return rvc->Insert ("HGISamplesVolume", _hgiSamplesVolume);
}

bool HGISamplesGenerationRenderPass::IsAvailable (const RenderScene* renderScenee, const Camera* camera,
	const RenderSettings& settings, const RenderVolumeCollection* rvc) const
{
	/*
	 * Always execute screen space directional occlusion samples generation render pass
	*/

	return true;
}

void HGISamplesGenerationRenderPass::Clear ()
{
	/*
	 * Clear screen space directional occlusion samples volume
	*/

	delete _hgiSamplesVolume;
}

void HGISamplesGenerationRenderPass::UpdateSamplesVolume (const RenderSettings& settings)
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
