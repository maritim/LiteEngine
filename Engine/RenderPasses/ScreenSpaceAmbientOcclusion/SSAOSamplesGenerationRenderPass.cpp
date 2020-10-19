#include "SSAOSamplesGenerationRenderPass.h"

SSAOSamplesGenerationRenderPass::SSAOSamplesGenerationRenderPass () :
	_ssaoSamplesVolume (nullptr)
{

}

void SSAOSamplesGenerationRenderPass::Init (const RenderSettings& settings)
{
	/*
	 * Initialize screen space ambient occlusion samples volume
	*/

	_ssaoSamplesVolume = new SSAOSamplesVolume (settings.ssao_samples);
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

	delete _ssaoSamplesVolume;
}

void SSAOSamplesGenerationRenderPass::UpdateSamplesVolume (const RenderSettings& settings)
{
	if (_ssaoSamplesVolume->GetSamplesCount () != settings.ssao_samples) {

		/*
		 * Clear screen space ambient occlusion samples volume
		*/

		delete _ssaoSamplesVolume;

		/*
		 * Create screen space ambient occlusion samples volume
		*/

		_ssaoSamplesVolume = new SSAOSamplesVolume (settings.ssao_samples);
	}
}
