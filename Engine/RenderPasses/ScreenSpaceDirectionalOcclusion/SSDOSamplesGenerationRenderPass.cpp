#include "SSDOSamplesGenerationRenderPass.h"

#include "Core/Console/Console.h"

SSDOSamplesGenerationRenderPass::SSDOSamplesGenerationRenderPass () :
	_ssdoSamplesVolume (nullptr)
{

}

void SSDOSamplesGenerationRenderPass::Init (const RenderSettings& settings)
{
	/*
	 * Create screen space directional occlusion samples volume
	*/

	_ssdoSamplesVolume = new SSDOSamplesVolume (settings.ssdo_samples);
}

RenderVolumeCollection* SSDOSamplesGenerationRenderPass::Execute (const RenderScene* renderScene, const Camera* camera,
	const RenderSettings& settings, RenderVolumeCollection* rvc)
{
	/*
	 * Update screen space directional occlusion samples volume
	*/

	UpdateSamplesVolume (settings);

	return rvc->Insert ("SSDOSamplesVolume", _ssdoSamplesVolume);
}

bool SSDOSamplesGenerationRenderPass::IsAvailable (const RenderScene* renderScenee, const Camera* camera,
	const RenderSettings& settings, const RenderVolumeCollection* rvc) const
{
	/*
	 * Always execute screen space directional occlusion samples generation render pass
	*/

	return true;
}

void SSDOSamplesGenerationRenderPass::Clear ()
{
	/*
	 * Clear screen space directional occlusion samples volume
	*/

	delete _ssdoSamplesVolume;
}

void SSDOSamplesGenerationRenderPass::UpdateSamplesVolume (const RenderSettings& settings)
{
	if (_ssdoSamplesVolume->GetSamplesCount () != settings.ssdo_samples) {

		/*
		 * Clear screen space ambient occlusion samples volume
		*/

		delete _ssdoSamplesVolume;

		/*
		 * Create screen space ambient occlusion samples volume
		*/

		_ssdoSamplesVolume = new SSDOSamplesVolume (settings.ssdo_samples);
	}
}
