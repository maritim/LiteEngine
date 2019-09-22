#include "SSDOSamplesGenerationRenderPass.h"

#include "Core/Console/Console.h"

SSDOSamplesGenerationRenderPass::SSDOSamplesGenerationRenderPass () :
	_ssdoSamplesVolume (new SSDOSamplesVolume ())
{

}

SSDOSamplesGenerationRenderPass::~SSDOSamplesGenerationRenderPass ()
{
	delete _ssdoSamplesVolume;
}

void SSDOSamplesGenerationRenderPass::Init (const RenderSettings& settings)
{
	/*
	 * Initialize screen space directional occlusion samples volume
	*/

	InitSamplesVolume (settings);
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

	_ssdoSamplesVolume->Clear ();
}

void SSDOSamplesGenerationRenderPass::UpdateSamplesVolume (const RenderSettings& settings)
{
	if (_ssdoSamplesVolume->GetSamplesCount () != settings.ssdo_samples) {

		/*
		 * Clear screen space ambient occlusion samples volume
		*/

		_ssdoSamplesVolume->Clear ();

		/*
		 * Initialize screen space ambient occlusion samples volume
		*/

		InitSamplesVolume (settings);
	}
}

void SSDOSamplesGenerationRenderPass::InitSamplesVolume (const RenderSettings& settings)
{
	if (!_ssdoSamplesVolume->Init (settings.ssdo_samples)) {
		Console::LogError (std::string () + "Screen space directional occlusion samples cannot be initialized! " +
			"It is not possible to continue the process. End now!");
		exit (SSDO_SAMPLES_NOT_INIT);
	}
}