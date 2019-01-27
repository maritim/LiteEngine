#include "GammaCorrectionContainerRenderSubPass.h"

#include "Systems/Settings/SettingsManager.h"

GammaCorrectionContainerRenderSubPass::GammaCorrectionContainerRenderSubPass () :
	_enabled (false)
{

}

GammaCorrectionContainerRenderSubPass::~GammaCorrectionContainerRenderSubPass ()
{
	ClearSettings ();
}

void GammaCorrectionContainerRenderSubPass::Init ()
{
	/*
	 *
	*/

	PostProcessContainerRenderSubPass::Init ();

	/*
	 * Initialize gamma correction settings
	*/

	InitSettings ();
}

bool GammaCorrectionContainerRenderSubPass::IsAvailable (const Scene* scene, const Camera* camera, const RenderVolumeCollection* rvc) const
{
	/*
	 * Check if gamma correction is enabled
	*/

	return _enabled;
}

void GammaCorrectionContainerRenderSubPass::Notify (Object* sender, const SettingsObserverArgs& args)
{
	std::string name = args.GetName ();

	/*
	 * Update gamma correction availability
	*/

	if (name == "gamma_correction") {
		_enabled = SettingsManager::Instance ()->GetValue<bool> ("gamma_correction", _enabled);
	}
}

std::string GammaCorrectionContainerRenderSubPass::GetPostProcessFragmentShaderPath () const
{
	return "Assets/Shaders/GammaCorrection/gammaCorrectionFragment.glsl";
}

std::string GammaCorrectionContainerRenderSubPass::GetPostProcessVolumeName () const
{
	return "PostProcessMapVolume";
}

PostProcessMapVolume* GammaCorrectionContainerRenderSubPass::CreatePostProcessVolume () const
{
	PostProcessMapVolume* volume = new PostProcessMapVolume ();

	return volume;
}

void GammaCorrectionContainerRenderSubPass::InitSettings ()
{
	/*
	 * Initialize gamma correction availability
	*/

	_enabled = SettingsManager::Instance ()->GetValue<bool> ("gamma_correction", _enabled);

	/*
	 * Attach to settings manager
	*/

	SettingsManager::Instance ()->Attach ("gamma_correction", this);
}

void GammaCorrectionContainerRenderSubPass::ClearSettings ()
{
	/*
	 * Detach
	*/

	SettingsManager::Instance ()->Detach ("gamma_correction", this);
}
