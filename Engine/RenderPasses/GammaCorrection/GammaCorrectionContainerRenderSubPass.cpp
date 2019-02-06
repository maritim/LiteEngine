#include "GammaCorrectionContainerRenderSubPass.h"

#include "Systems/Window/Window.h"
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
	 * Initialize gamma correction settings
	*/

	InitSettings ();

	/*
	 *
	*/

	PostProcessContainerRenderSubPass::Init ();
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

void GammaCorrectionContainerRenderSubPass::Clear ()
{
	/*
	 *
	*/

	PostProcessContainerRenderSubPass::Clear ();

	/*
	 * Clear settings
	*/

	ClearSettings ();
}

std::string GammaCorrectionContainerRenderSubPass::GetPostProcessFragmentShaderPath () const
{
	return "Assets/Shaders/GammaCorrection/gammaCorrectionFragment.glsl";
}

std::string GammaCorrectionContainerRenderSubPass::GetPostProcessVolumeName () const
{
	return "PostProcessMapVolume";
}

glm::ivec2 GammaCorrectionContainerRenderSubPass::GetPostProcessVolumeResolution () const
{
	return glm::ivec2 (Window::GetWidth (), Window::GetHeight ());
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
