#include "SSAOSamplesGenerationContainerRenderSubPass.h"

#include "Systems/Settings/SettingsManager.h"

#include "Core/Console/Console.h"

SSAOSamplesGenerationContainerRenderSubPass::SSAOSamplesGenerationContainerRenderSubPass () :
	_enabled (false),
	_samplesSize (0),
	_ssaoSamplesVolume (new SSAOSamplesVolume ())
{

}

SSAOSamplesGenerationContainerRenderSubPass::~SSAOSamplesGenerationContainerRenderSubPass ()
{
	delete _ssaoSamplesVolume;
}

void SSAOSamplesGenerationContainerRenderSubPass::Init ()
{
	/*
	 * Initialize screen space ambient occlusion samples settings
	*/

	InitSettings ();

	/*
	 * Initialize screen space ambient occlusion samples volume
	*/

	InitSamplesVolume ();
}

RenderVolumeCollection* SSAOSamplesGenerationContainerRenderSubPass::Execute (const Scene* scene, const Camera* camera, RenderVolumeCollection* rvc)
{
	return rvc->InsertScoped ("SSAOSamplesVolume", _ssaoSamplesVolume);
}

bool SSAOSamplesGenerationContainerRenderSubPass::IsAvailable (const Scene* scene, const Camera* camera, const RenderVolumeCollection* rvc) const
{
	/*
	 * Check if screen space ambient occlusion is enabled
	*/

	return _enabled;
}

void SSAOSamplesGenerationContainerRenderSubPass::Notify (Object* sender, const SettingsObserverArgs& args)
{
	std::string name = args.GetName ();

	/*
	 * Update screen space ambient occlusion availability
	*/

	if (name == "ambient_occlusion") {
		_enabled = SettingsManager::Instance ()->GetValue<bool> ("ambient_occlusion", _enabled);
	}

	/*
	 * Update screen space ambient occlusion samples size
	*/

	if (name == "ssao_samples") {
		_samplesSize = SettingsManager::Instance ()->GetValue<int> ("ssao_samples", _samplesSize);

		/*
		 * Clear sceen space ambient occlusion samples volume
		*/

		_ssaoSamplesVolume->Clear ();

		/*
		 * Update screen space ambient occlusion samples volume
		*/

		InitSamplesVolume ();
	}
}

void SSAOSamplesGenerationContainerRenderSubPass::Clear ()
{
	/*
	 * Clear screen space ambient occlusion samples volume
	*/

	_ssaoSamplesVolume->Clear ();

	/*
	 * Clear settings
	*/

	ClearSettings ();
}

void SSAOSamplesGenerationContainerRenderSubPass::InitSettings ()
{
	/*
	 * Initialize screen space ambient occlusion availability
	*/

	_enabled = SettingsManager::Instance ()->GetValue<bool> ("ambient_occlusion", _enabled);

	/*
	 * Initialize screen space ambient occlusion samples size
	*/

	_samplesSize = SettingsManager::Instance ()->GetValue<int> ("ssao_samples", _samplesSize);

	/*
	 * Attach to settings manager
	*/

	SettingsManager::Instance ()->Attach ("ambient_occlusion", this);
	SettingsManager::Instance ()->Attach ("ssao_samples", this);
}

void SSAOSamplesGenerationContainerRenderSubPass::ClearSettings ()
{
	/*
	 * Detach
	*/

	SettingsManager::Instance ()->Detach ("ambient_occlusion", this);
	SettingsManager::Instance ()->Detach ("ssao_samples", this);
}

void SSAOSamplesGenerationContainerRenderSubPass::InitSamplesVolume ()
{
	if (!_ssaoSamplesVolume->Init (_samplesSize)) {
		Console::LogError (std::string () + "Screen space ambient occlusion samples cannot be initialized! " +
			"It is not possible to continue the process. End now!");
		exit (SSAO_SAMPLES_NOT_INIT);
	}
}