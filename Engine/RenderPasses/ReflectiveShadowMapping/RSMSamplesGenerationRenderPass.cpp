#include "RSMSamplesGenerationRenderPass.h"

#include "Core/Console/Console.h"

RSMSamplesGenerationRenderPass::RSMSamplesGenerationRenderPass () :
	_reflectiveShadowMapSamplesVolume (new RSMSamplesVolume ())
{

}

RSMSamplesGenerationRenderPass::~RSMSamplesGenerationRenderPass ()
{
	delete _reflectiveShadowMapSamplesVolume;
}

void RSMSamplesGenerationRenderPass::Init (const RenderSettings& settings)
{
	/*
	 * Initialize reflective shadow map samples volume
	*/

	InitRSMSamplesVolume (settings);
}

RenderVolumeCollection* RSMSamplesGenerationRenderPass::Execute (const RenderScene* renderScene, const Camera* camera,
	const RenderSettings& settings, RenderVolumeCollection* rvc)
{
	/*
	 * Update volume
	*/

	UpdateRSMSamplesVolume (settings);

	return rvc->Insert ("ReflectiveShadowMapSamplesVolume", _reflectiveShadowMapSamplesVolume);
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
	_reflectiveShadowMapSamplesVolume->Clear ();
}

void RSMSamplesGenerationRenderPass::InitRSMSamplesVolume (const RenderSettings& settings)
{
	if (!_reflectiveShadowMapSamplesVolume->Init(settings.rsm_samples)) {
		Console::LogError(std::string () + "Reflective shadow map samples cannot be initialized! " +
			"It is not possible to continue the process. End now!");
		exit(REFLECTIVE_SHADOW_MAP_SAMPLES_NOT_INIT);
	}
}

void RSMSamplesGenerationRenderPass::UpdateRSMSamplesVolume (const RenderSettings& settings)
{
	std::size_t rsmSamplesSize = _reflectiveShadowMapSamplesVolume->GetSize ();

	if (rsmSamplesSize != settings.rsm_samples) {

		/*
		 * Clear reflective shadow map samples
		*/

		_reflectiveShadowMapSamplesVolume->Clear ();

		/*
		 * Initialize reflective shadow map samples
		*/

		InitRSMSamplesVolume (settings);
	}
}