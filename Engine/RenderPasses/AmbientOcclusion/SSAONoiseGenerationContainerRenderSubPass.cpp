#include "SSAONoiseGenerationContainerRenderSubPass.h"

#include "Core/Console/Console.h"

SSAONoiseGenerationContainerRenderSubPass::SSAONoiseGenerationContainerRenderSubPass () :
	_ssaoNoiseMapVolume (new SSAONoiseMapVolume ())
{

}

SSAONoiseGenerationContainerRenderSubPass::~SSAONoiseGenerationContainerRenderSubPass ()
{
	delete _ssaoNoiseMapVolume;
}

void SSAONoiseGenerationContainerRenderSubPass::Init (const RenderSettings& settings)
{
	/*
	 * Initialize screen space ambient occlusion noise map volume
	*/

	InitNoiseMapVolume (settings);
}

RenderVolumeCollection* SSAONoiseGenerationContainerRenderSubPass::Execute (const RenderScene* renderScene, const Camera* camera,
	const RenderSettings& settings, RenderVolumeCollection* rvc)
{
	/*
	 * Update screen space ambient occlusion noise map volume
	*/

	UpdateNoiseMapVolume (settings);

	return rvc->Insert ("SSAONoiseMapVolume", _ssaoNoiseMapVolume);
}

void SSAONoiseGenerationContainerRenderSubPass::UpdateNoiseMapVolume (const RenderSettings& settings)
{
	glm::ivec2 noiseMapResolution = glm::ivec2 (settings.ssao_noise_size);

	if (noiseMapResolution != _ssaoNoiseMapVolume->GetSize ()) {

		/*
		 * Clear screen space ambient occlusion noise map volume
		*/

		_ssaoNoiseMapVolume->Clear ();

		/*
		 * Initialize screen space ambient occlusion noise map volume
		*/

		InitNoiseMapVolume (settings);
	}
}

void SSAONoiseGenerationContainerRenderSubPass::Clear ()
{
	/*
	 * Clear screen space ambient occlusion noise map volume
	*/

	_ssaoNoiseMapVolume->Clear ();
}

bool SSAONoiseGenerationContainerRenderSubPass::IsAvailable (const RenderScene* renderScene, const Camera* camera,
	const RenderSettings& settings, const RenderVolumeCollection* rvc) const
{
	/*
	 * Check if screen space ambient occlusion is enabled
	*/

	return settings.ssao_enabled;
}

void SSAONoiseGenerationContainerRenderSubPass::InitNoiseMapVolume (const RenderSettings& settings)
{
	if (!_ssaoNoiseMapVolume->Init (settings.ssao_noise_size, settings.ssao_noise_size)) {
		Console::LogError (std::string () + "Screen space ambient occlusion samples cannot be initialized! " +
			"It is not possible to continue the process. End now!");
		exit (SSAO_NOISE_NOT_INIT);
	}
}