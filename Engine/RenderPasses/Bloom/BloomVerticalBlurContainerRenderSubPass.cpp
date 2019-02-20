#include "BloomVerticalBlurContainerRenderSubPass.h"

#include "Systems/Settings/SettingsManager.h"

BloomVerticalBlurContainerRenderSubPass::BloomVerticalBlurContainerRenderSubPass () :
	_enabled (false),
	_resolution (0)
{

}

BloomVerticalBlurContainerRenderSubPass::~BloomVerticalBlurContainerRenderSubPass ()
{

}

void BloomVerticalBlurContainerRenderSubPass::Init ()
{
	/*
	 * Initialize bloom settings
	*/

	InitSettings ();

	/*
	 *
	*/

	VerticalGaussianBlurContainerRenderSubPass::Init ();
}

bool BloomVerticalBlurContainerRenderSubPass::IsAvailable (const Scene* scene, const Camera* camera, const RenderVolumeCollection* rvc) const
{
	/*
	 * Check if bloom is enabled
	*/

	return _enabled;
}

void BloomVerticalBlurContainerRenderSubPass::Notify (Object* sender, const SettingsObserverArgs& args)
{
	std::string name = args.GetName ();

	/*
	 * Update bloom availability
	*/

	if (name == "bloom") {
		_enabled = SettingsManager::Instance ()->GetValue<bool> ("bloom", _enabled);
	}

	/*
	 * Update bloom resolution
	*/

	if (name == "bloom_resolution") {
		_resolution = SettingsManager::Instance ()->GetValue<glm::vec2> ("bloom_resolution", (glm::vec2) _resolution);

		/*
		 * Initialize bloom map
		*/

		ReinitPostProcessVolume ();
	}
}

void BloomVerticalBlurContainerRenderSubPass::Clear ()
{
	/*
	 *
	*/

	VerticalGaussianBlurContainerRenderSubPass::Clear ();

	/*
	 * Clear settings
	*/

	ClearSettings ();
}

glm::ivec2 BloomVerticalBlurContainerRenderSubPass::GetPostProcessVolumeResolution () const
{
	return _resolution;
}

void BloomVerticalBlurContainerRenderSubPass::InitSettings ()
{
	/*
	 * Initialize bloom availability
	*/

	_enabled = SettingsManager::Instance ()->GetValue<bool> ("bloom", _enabled);

	/*
	 * Initialize bloomresolution
	*/

	_resolution = SettingsManager::Instance ()->GetValue<glm::vec2> ("bloom_resolution", (glm::vec2) _resolution);

	/*
	 * Attach to settings manager
	*/

	SettingsManager::Instance ()->Attach ("bloom", this);
	SettingsManager::Instance ()->Attach ("bloom_resolution", this);
}

void BloomVerticalBlurContainerRenderSubPass::ClearSettings ()
{
	/*
	 * Detach
	*/

	SettingsManager::Instance ()->Detach ("bloom", this);
	SettingsManager::Instance ()->Detach ("bloom_resolution", this);
}
