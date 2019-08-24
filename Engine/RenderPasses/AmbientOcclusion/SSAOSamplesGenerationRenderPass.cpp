#include "SSAOSamplesGenerationRenderPass.h"

#include "Core/Console/Console.h"

SSAOSamplesGenerationRenderPass::SSAOSamplesGenerationRenderPass () :
	_ssaoSamplesVolume (new SSAOSamplesVolume ())
{

}

SSAOSamplesGenerationRenderPass::~SSAOSamplesGenerationRenderPass ()
{
	delete _ssaoSamplesVolume;
}

void SSAOSamplesGenerationRenderPass::Init (const RenderSettings& settings)
{
	/*
	 * Initialize screen space ambient occlusion samples volume
	*/

	InitSamplesVolume (settings);
}

RenderVolumeCollection* SSAOSamplesGenerationRenderPass::Execute (const RenderScene* renderScene, const Camera* camera,
	const RenderSettings& settings, RenderVolumeCollection* rvc)
{
	/*
	 * Update screen space ambient occlusion samples volume
	*/

	UpdateSamplesVolume (settings);

	return rvc->Insert ("SSAOSamplesVolume", _ssaoSamplesVolume);
}

bool SSAOSamplesGenerationRenderPass::IsAvailable (const RenderScene* renderScenee, const Camera* camera,
	const RenderSettings& settings, const RenderVolumeCollection* rvc) const
{
	/*
	 * Check if screen space ambient occlusion is enabled
	*/

	return settings.ssao_enabled;
}

void SSAOSamplesGenerationRenderPass::Clear ()
{
	/*
	 * Clear screen space ambient occlusion samples volume
	*/

	_ssaoSamplesVolume->Clear ();
}

void SSAOSamplesGenerationRenderPass::UpdateSamplesVolume (const RenderSettings& settings)
{
	if (_ssaoSamplesVolume->GetSamplesCount () != settings.ssao_samples) {

		/*
		 * Clear screen space ambient occlusion samples volume
		*/

		_ssaoSamplesVolume->Clear ();

		/*
		 * Initialize screen space ambient occlusion samples volume
		*/

		InitSamplesVolume (settings);
	}
}

void SSAOSamplesGenerationRenderPass::InitSamplesVolume (const RenderSettings& settings)
{
	if (!_ssaoSamplesVolume->Init (settings.ssao_samples)) {
		Console::LogError (std::string () + "Screen space ambient occlusion samples cannot be initialized! " +
			"It is not possible to continue the process. End now!");
		exit (SSAO_SAMPLES_NOT_INIT);
	}
}