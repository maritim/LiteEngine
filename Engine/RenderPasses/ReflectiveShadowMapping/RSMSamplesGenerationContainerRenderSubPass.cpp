#include "RSMSamplesGenerationContainerRenderSubPass.h"

#include "Core/Console/Console.h"

RSMSamplesGenerationContainerRenderSubPass::RSMSamplesGenerationContainerRenderSubPass () :
	_reflectiveShadowMapSamplesVolume (new RSMSamplesVolume ())
{

}

RSMSamplesGenerationContainerRenderSubPass::~RSMSamplesGenerationContainerRenderSubPass ()
{
	delete _reflectiveShadowMapSamplesVolume;
}

void RSMSamplesGenerationContainerRenderSubPass::Init (const RenderSettings& settings)
{
	/*
	 * Initialize reflective shadow map samples volume
	*/

	InitRSMSamplesVolume (settings);
}

RenderVolumeCollection* RSMSamplesGenerationContainerRenderSubPass::Execute (const RenderScene* renderScene, const Camera* camera,
	const RenderSettings& settings, RenderVolumeCollection* rvc)
{
	/*
	 * Update volume
	*/

	UpdateRSMSamplesVolume (settings);

	return rvc->Insert ("ReflectiveShadowMapSamplesVolume", _reflectiveShadowMapSamplesVolume);
}

bool RSMSamplesGenerationContainerRenderSubPass::IsAvailable (const RenderLightObject* renderLightObject) const
{
	/*
	 * Always execute reflective shadow map accumulation sub pass
	*/

	return true;
}

void RSMSamplesGenerationContainerRenderSubPass::Clear ()
{
	_reflectiveShadowMapSamplesVolume->Clear ();
}

void RSMSamplesGenerationContainerRenderSubPass::InitRSMSamplesVolume (const RenderSettings& settings)
{
	if (!_reflectiveShadowMapSamplesVolume->Init(settings.rsm_samples)) {
		Console::LogError(std::string () + "Reflective shadow map samples cannot be initialized! " +
			"It is not possible to continue the process. End now!");
		exit(REFLECTIVE_SHADOW_MAP_SAMPLES_NOT_INIT);
	}
}

void RSMSamplesGenerationContainerRenderSubPass::UpdateRSMSamplesVolume (const RenderSettings& settings)
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