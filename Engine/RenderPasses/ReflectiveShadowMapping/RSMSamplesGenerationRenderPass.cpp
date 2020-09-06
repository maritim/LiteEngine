#include "RSMSamplesGenerationRenderPass.h"

#include "Core/Console/Console.h"

RSMSamplesGenerationRenderPass::RSMSamplesGenerationRenderPass () :
	_rsmSamplesVolume (nullptr)
{

}

void RSMSamplesGenerationRenderPass::Init (const RenderSettings& settings)
{
	/*
	 * Initialize reflective shadow map samples volume
	*/

	_rsmSamplesVolume = new RSMSamplesVolume (settings.rsm_samples);
}

RenderVolumeCollection* RSMSamplesGenerationRenderPass::Execute (const RenderScene* renderScene, const Camera* camera,
	const RenderSettings& settings, RenderVolumeCollection* rvc)
{
	/*
	 * Update volume
	*/

	UpdateRSMSamplesVolume (settings);

	return rvc->Insert ("ReflectiveShadowMapSamplesVolume", _rsmSamplesVolume);
}

bool RSMSamplesGenerationRenderPass::IsAvailable (const RenderLightObject* renderLightObject) const
{
	/*
	 * Always execute reflective shadow map accumulation sub pass
	*/

	return true;
}

void RSMSamplesGenerationRenderPass::Clear ()
{
	delete _rsmSamplesVolume;
}

void RSMSamplesGenerationRenderPass::UpdateRSMSamplesVolume (const RenderSettings& settings)
{
	std::size_t rsmSamplesSize = _rsmSamplesVolume->GetSize ();

	if (rsmSamplesSize != settings.rsm_samples) {

		/*
		 * Clear reflective shadow map samples
		*/

		delete _rsmSamplesVolume;

		/*
		 * Initialize reflective shadow map samples
		*/

		_rsmSamplesVolume = new RSMSamplesVolume (settings.rsm_samples);
	}
}
