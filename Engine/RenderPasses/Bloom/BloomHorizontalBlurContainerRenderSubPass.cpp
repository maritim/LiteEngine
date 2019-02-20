#include "BloomHorizontalBlurContainerRenderSubPass.h"

#include "Systems/Settings/SettingsManager.h"

BloomHorizontalBlurContainerRenderSubPass::BloomHorizontalBlurContainerRenderSubPass () :
	_enabled (false),
	_resolution (0)
{

}

BloomHorizontalBlurContainerRenderSubPass::~BloomHorizontalBlurContainerRenderSubPass ()
{

}

void BloomHorizontalBlurContainerRenderSubPass::Init ()
{
	/*
	 * Initialize bloom settings
	*/

	InitSettings ();

	/*
	 *
	*/

	HorizontalGaussianBlurContainerRenderSubPass::Init ();
}

bool BloomHorizontalBlurContainerRenderSubPass::IsAvailable (const Scene* scene, const Camera* camera, const RenderVolumeCollection* rvc) const
{
	/*
	 * Check if bloom is enabled
	*/

	return _enabled;
}

void BloomHorizontalBlurContainerRenderSubPass::Notify (Object* sender, const SettingsObserverArgs& args)
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

void BloomHorizontalBlurContainerRenderSubPass::Clear ()
{
	/*
	 *
	*/

	HorizontalGaussianBlurContainerRenderSubPass::Clear ();

	/*
	 * Clear settings
	*/

	ClearSettings ();
}

glm::ivec2 BloomHorizontalBlurContainerRenderSubPass::GetPostProcessVolumeResolution () const
{
	return _resolution;
}

void BloomHorizontalBlurContainerRenderSubPass::InitSettings ()
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

void BloomHorizontalBlurContainerRenderSubPass::ClearSettings ()
{
	/*
	 * Detach
	*/

	SettingsManager::Instance ()->Detach ("bloom", this);
	SettingsManager::Instance ()->Detach ("bloom_resolution", this);
}
