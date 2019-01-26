#include "SSAONoiseGenerationContainerRenderSubPass.h"

#include "Systems/Settings/SettingsManager.h"

#include "Core/Console/Console.h"

SSAONoiseGenerationContainerRenderSubPass::SSAONoiseGenerationContainerRenderSubPass () :
	_enabled (false),
	_ssaoNoiseMapResolution (0.0f),
	_ssaoNoiseMapVolume (new SSAONoiseMapVolume ())
{

}

SSAONoiseGenerationContainerRenderSubPass::~SSAONoiseGenerationContainerRenderSubPass ()
{
	delete _ssaoNoiseMapVolume;

	/*
	 * Clear settings
	*/

	ClearSettings ();
}

void SSAONoiseGenerationContainerRenderSubPass::Init ()
{
	/*
	 *
	*/

	InitSettings ();

	/*
	 * Initialize screen space ambient occlusion noise map volume
	*/

	InitNoiseMapVolume ();
}

RenderVolumeCollection* SSAONoiseGenerationContainerRenderSubPass::Execute (const Scene* scene, const Camera* camera, RenderVolumeCollection* rvc)
{
	return rvc->InsertScoped ("SSAONoiseMapVolume", _ssaoNoiseMapVolume);
}

bool SSAONoiseGenerationContainerRenderSubPass::IsAvailable (const Scene* scene, const Camera* camera, const RenderVolumeCollection* rvc) const
{
	/*
	 * Check if screen space ambient occlusion is enabled
	*/

	return _enabled;
}

void SSAONoiseGenerationContainerRenderSubPass::Notify (Object* sender, const SettingsObserverArgs& args)
{
	std::string name = args.GetName ();

	/*
	 * Update screen space ambient occlusion availability
	*/

	if (name == "ambient_occlusion") {
		_enabled = SettingsManager::Instance ()->GetValue<bool> ("ambient_occlusion", _enabled);
	}

	/*
	 * Update screen space ambient occlusion noise map resolution
	*/

	if (name == "ssao_noise_resolution") {
		_ssaoNoiseMapResolution = SettingsManager::Instance ()->GetValue<glm::vec2> ("ssao_noise_resolution", _ssaoNoiseMapResolution);

		/*
		 * Update screen space ambient occlusion noise map volume
		*/

		InitNoiseMapVolume ();
	}
}

void SSAONoiseGenerationContainerRenderSubPass::InitSettings ()
{
	/*
	 * Initialize screen space ambient occlusion availability
	*/

	_enabled = SettingsManager::Instance ()->GetValue<bool> ("ambient_occlusion", _enabled);

	/*
	 * Initialize screen space ambient occlusion noise map resolution
	*/

	_ssaoNoiseMapResolution = SettingsManager::Instance ()->GetValue<glm::vec2> ("ssao_noise_resolution", _ssaoNoiseMapResolution);

	/*
	 * Attach to settings manager
	*/

	SettingsManager::Instance ()->Attach ("ambient_occlusion", this);
	SettingsManager::Instance ()->Attach ("ssao_noise_resolution", this);
}

void SSAONoiseGenerationContainerRenderSubPass::ClearSettings ()
{
	/*
	 * Detach
	*/

	SettingsManager::Instance ()->Detach ("ambient_occlusion", this);
	SettingsManager::Instance ()->Detach ("ssao_noise_resolution", this);
}

void SSAONoiseGenerationContainerRenderSubPass::InitNoiseMapVolume ()
{
	if (!_ssaoNoiseMapVolume->Init (_ssaoNoiseMapResolution.x, _ssaoNoiseMapResolution.y)) {
		Console::LogError (std::string () + "Screen space ambient occlusion samples cannot be initialized! " +
			"It is not possible to continue the process. End now!");
		exit (SSAO_NOISE_NOT_INIT);
	}
}