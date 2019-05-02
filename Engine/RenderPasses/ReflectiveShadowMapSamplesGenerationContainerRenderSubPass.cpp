#include "ReflectiveShadowMapSamplesGenerationContainerRenderSubPass.h"

#include "Systems/Settings/SettingsManager.h"

#include "Core/Console/Console.h"

ReflectiveShadowMapSamplesGenerationContainerRenderSubPass::ReflectiveShadowMapSamplesGenerationContainerRenderSubPass () :
	_samplesSize (0),
	_reflectiveShadowMapSamplesVolume (new ReflectiveShadowMapSamplesVolume ())
{

}

ReflectiveShadowMapSamplesGenerationContainerRenderSubPass::~ReflectiveShadowMapSamplesGenerationContainerRenderSubPass ()
{
	delete _reflectiveShadowMapSamplesVolume;
}

void ReflectiveShadowMapSamplesGenerationContainerRenderSubPass::Init (const RenderSettings& settings)
{
	/*
	 * Initialize reflective shadow map samples settings
	*/

	InitSettings ();

	/*
	 * Initialize reflective shadow map samples volume
	*/

	InitRSMSamplesVolume ();
}

RenderVolumeCollection* ReflectiveShadowMapSamplesGenerationContainerRenderSubPass::Execute (const Scene* scene, const Camera* camera,
	const RenderSettings& settings, RenderVolumeCollection* rvc)
{
	return rvc->Insert ("ReflectiveShadowMapSamplesVolume", _reflectiveShadowMapSamplesVolume);
}

bool ReflectiveShadowMapSamplesGenerationContainerRenderSubPass::IsAvailable (const VolumetricLight* volumetricLight) const
{
	/*
	 * Always execute reflective shadow map accumulation sub pass
	*/

	return true;
}

void ReflectiveShadowMapSamplesGenerationContainerRenderSubPass::Notify (Object* sender, const SettingsObserverArgs& args)
{
	std::string name = args.GetName ();

	/*
	 * Update reflective shadow map samples size
	*/

	if (name == "rsm_samples") {
		_samplesSize = SettingsManager::Instance ()->GetValue<int> ("rsm_samples", _samplesSize);

		/*
		 * Clear reflective shadow map samples volume
		*/

		_reflectiveShadowMapSamplesVolume->Clear ();

		/*
		 * Initialize reflective shadow map samples volume
		*/

		InitRSMSamplesVolume ();
	}
}

void ReflectiveShadowMapSamplesGenerationContainerRenderSubPass::Clear ()
{
	_reflectiveShadowMapSamplesVolume->Clear ();

	/*
	 * Clear settings
	*/

	ClearSettings ();
}

void ReflectiveShadowMapSamplesGenerationContainerRenderSubPass::InitSettings ()
{
	/*
	 * Initialize reflective shadow map samples size
	*/

	_samplesSize = SettingsManager::Instance ()->GetValue<int> ("rsm_samples", _samplesSize);

	/*
	 * Attach to settings manager
	*/

	SettingsManager::Instance ()->Attach ("rsm_samples", this);
}

void ReflectiveShadowMapSamplesGenerationContainerRenderSubPass::ClearSettings ()
{
	/*
	 * Detach
	*/

	SettingsManager::Instance ()->Detach ("rsm_samples", this);
}

void ReflectiveShadowMapSamplesGenerationContainerRenderSubPass::InitRSMSamplesVolume ()
{
	if (!_reflectiveShadowMapSamplesVolume->Init(_samplesSize)) {
		Console::LogError(std::string () + "Reflective shadow map samples cannot be initialized! " +
			"It is not possible to continue the process. End now!");
		exit(REFLECTIVE_SHADOW_MAP_SAMPLES_NOT_INIT);
	}
}
