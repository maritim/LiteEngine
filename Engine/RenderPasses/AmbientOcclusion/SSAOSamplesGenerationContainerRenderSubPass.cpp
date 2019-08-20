#include "SSAOSamplesGenerationContainerRenderSubPass.h"

#include "Core/Console/Console.h"

SSAOSamplesGenerationContainerRenderSubPass::SSAOSamplesGenerationContainerRenderSubPass () :
	_ssaoSamplesVolume (new SSAOSamplesVolume ())
{

}

SSAOSamplesGenerationContainerRenderSubPass::~SSAOSamplesGenerationContainerRenderSubPass ()
{
	delete _ssaoSamplesVolume;
}

void SSAOSamplesGenerationContainerRenderSubPass::Init (const RenderSettings& settings)
{
	/*
	 * Initialize screen space ambient occlusion samples volume
	*/

	InitSamplesVolume (settings);
}

RenderVolumeCollection* SSAOSamplesGenerationContainerRenderSubPass::Execute (const RenderScene* renderScene, const Camera* camera,
	const RenderSettings& settings, RenderVolumeCollection* rvc)
{
	/*
	 * Update screen space ambient occlusion samples volume
	*/

	UpdateSamplesVolume (settings);

	return rvc->Insert ("SSAOSamplesVolume", _ssaoSamplesVolume);
}

bool SSAOSamplesGenerationContainerRenderSubPass::IsAvailable (const RenderScene* renderScenee, const Camera* camera,
	const RenderSettings& settings, const RenderVolumeCollection* rvc) const
{
	/*
	 * Check if screen space ambient occlusion is enabled
	*/

	return settings.ssao_enabled;
}

void SSAOSamplesGenerationContainerRenderSubPass::Clear ()
{
	/*
	 * Clear screen space ambient occlusion samples volume
	*/

	_ssaoSamplesVolume->Clear ();
}

void SSAOSamplesGenerationContainerRenderSubPass::UpdateSamplesVolume (const RenderSettings& settings)
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

void SSAOSamplesGenerationContainerRenderSubPass::InitSamplesVolume (const RenderSettings& settings)
{
	if (!_ssaoSamplesVolume->Init (settings.ssao_samples)) {
		Console::LogError (std::string () + "Screen space ambient occlusion samples cannot be initialized! " +
			"It is not possible to continue the process. End now!");
		exit (SSAO_SAMPLES_NOT_INIT);
	}
}