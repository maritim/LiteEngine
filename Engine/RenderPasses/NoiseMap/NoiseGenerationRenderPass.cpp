#include "NoiseGenerationRenderPass.h"

#include "Core/Console/Console.h"

NoiseGenerationRenderPass::NoiseGenerationRenderPass () :
	_noiseMapVolume (nullptr)
{

}

void NoiseGenerationRenderPass::Init (const RenderSettings& settings)
{
	/*
	 * Initialize screen space ambient occlusion noise map volume
	*/

	_noiseMapVolume = CreateNoiseMapVolume (settings);
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
	auto size = _noiseMapVolume->GetTexture (0)->GetSize ();

	if ((std::size_t) noiseMapResolution.x != size.width ||
		(std::size_t) noiseMapResolution.y != size.height) {

		/*
		 * Clear screen space ambient occlusion noise map volume
		*/

		delete _noiseMapVolume;

		/*
		 * Initialize screen space ambient occlusion noise map volume
		*/

		_noiseMapVolume = CreateNoiseMapVolume (settings);
	}
}

void NoiseGenerationRenderPass::Clear ()
{
	/*
	 * Clear screen space ambient occlusion noise map volume
	*/

	delete _noiseMapVolume;
}
