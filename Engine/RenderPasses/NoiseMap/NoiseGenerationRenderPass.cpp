#include "NoiseGenerationRenderPass.h"

#include "Core/Console/Console.h"

NoiseGenerationRenderPass::NoiseGenerationRenderPass () :
	_noiseMapVolume (new NoiseMapVolume ())
{

}

NoiseGenerationRenderPass::~NoiseGenerationRenderPass ()
{
	delete _noiseMapVolume;
}

void NoiseGenerationRenderPass::Init (const RenderSettings& settings)
{
	/*
	 * Initialize screen space ambient occlusion noise map volume
	*/

	InitNoiseMapVolume (settings);
}

RenderVolumeCollection* NoiseGenerationRenderPass::Execute (const RenderScene* renderScene, const Camera* camera,
	const RenderSettings& settings, RenderVolumeCollection* rvc)
{
	/*
	 * Update screen space ambient occlusion noise map volume
	*/

	UpdateNoiseMapVolume (settings);

	return rvc->Insert ("NoiseMapVolume", _noiseMapVolume);
}

void NoiseGenerationRenderPass::UpdateNoiseMapVolume (const RenderSettings& settings)
{
	glm::ivec2 noiseMapResolution = GetNoiseMapVolumeResolution (settings);

	if (noiseMapResolution != _noiseMapVolume->GetSize ()) {

		/*
		 * Clear screen space ambient occlusion noise map volume
		*/

		_noiseMapVolume->Clear ();

		/*
		 * Initialize screen space ambient occlusion noise map volume
		*/

		InitNoiseMapVolume (settings);
	}
}

void NoiseGenerationRenderPass::Clear ()
{
	/*
	 * Clear screen space ambient occlusion noise map volume
	*/

	_noiseMapVolume->Clear ();
}

void NoiseGenerationRenderPass::InitNoiseMapVolume (const RenderSettings& settings)
{
	glm::ivec2 noiseMapResolution = GetNoiseMapVolumeResolution (settings);

	if (!_noiseMapVolume->Init (noiseMapResolution.x, noiseMapResolution.y)) {
		Console::LogError (std::string () + "Noise map cannot be initialized! " +
			"It is not possible to continue the process. End now!");
		exit (NOISE_NOT_INIT);
	}
}