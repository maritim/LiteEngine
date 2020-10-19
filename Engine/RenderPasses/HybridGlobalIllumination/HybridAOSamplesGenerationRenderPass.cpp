#include "HybridAOSamplesGenerationRenderPass.h"

void HybridAOSamplesGenerationRenderPass::Init (const RenderSettings& settings)
{
	/*
	 * Initialize screen space ambient occlusion samples volume
	*/

	_ssaoSamplesVolume = new SSAOSamplesVolume (settings.hgi_ao_samples);
}

bool HybridAOSamplesGenerationRenderPass::IsAvailable (const RenderScene* renderScenee, const Camera* camera,
	const RenderSettings& settings, const RenderVolumeCollection* rvc) const
{
	/*
	 * Check if screen space ambient occlusion is enabled
	*/

	return true;
}

void HybridAOSamplesGenerationRenderPass::UpdateSamplesVolume (const RenderSettings& settings)
{
	if (_ssaoSamplesVolume->GetSamplesCount () != settings.hgi_ao_samples) {

		/*
		 * Clear screen space ambient occlusion samples volume
		*/

		delete _ssaoSamplesVolume;

		/*
		 * Create screen space ambient occlusion samples volume
		*/

		_ssaoSamplesVolume = new SSAOSamplesVolume (settings.hgi_ao_samples);
	}
}
