#include "SSRAccumulationContainerRenderSubPass.h"

#include "Systems/Settings/SettingsManager.h"

#include "Systems/Window/Window.h"

SSRAccumulationContainerRenderSubPass::SSRAccumulationContainerRenderSubPass () :
	_enabled (false)
{

}

SSRAccumulationContainerRenderSubPass::~SSRAccumulationContainerRenderSubPass ()
{

}

void SSRAccumulationContainerRenderSubPass::Init ()
{
	/*
	 * Initialize screen space ambient occlusion settings
	*/

	InitSettings ();

	/*
	 *
	*/

	PostProcessContainerRenderSubPass::Init ();
}

bool SSRAccumulationContainerRenderSubPass::IsAvailable (const Scene* scene, const Camera* camera, const RenderVolumeCollection* rvc) const
{
	/*
	 * Check if screen space ambient occlusion is enabled
	*/

	return _enabled;
}

void SSRAccumulationContainerRenderSubPass::Notify (Object* sender, const SettingsObserverArgs& args)
{
	std::string name = args.GetName ();

	/*
	 * Update screen space ambient occlusion availability
	*/

	if (name == "screen_space_reflection") {
		_enabled = SettingsManager::Instance ()->GetValue<bool> ("screen_space_reflection", _enabled);
	}
}

void SSRAccumulationContainerRenderSubPass::Clear ()
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

std::string SSRAccumulationContainerRenderSubPass::GetPostProcessFragmentShaderPath () const
{
	return "Assets/Shaders/ScreenSpaceReflection/screenSpaceReflectionAccumulationFragment.glsl";
}

std::string SSRAccumulationContainerRenderSubPass::GetPostProcessVolumeName () const
{
	return "PostProcessMapVolume";
}

glm::ivec2 SSRAccumulationContainerRenderSubPass::GetPostProcessVolumeResolution () const
{
	return glm::ivec2 (Window::GetWidth (), Window::GetHeight ());
}

PostProcessMapVolume* SSRAccumulationContainerRenderSubPass::CreatePostProcessVolume () const
{
	PostProcessMapVolume* postProcessMapVolume = new PostProcessMapVolume ();

	return postProcessMapVolume;
}

void SSRAccumulationContainerRenderSubPass::InitSettings ()
{
	/*
	 * Initialize screen space reflection availability
	*/

	_enabled = SettingsManager::Instance ()->GetValue<bool> ("screen_space_reflection", _enabled);

	/*
	 * Attach to settings manager
	*/

	SettingsManager::Instance ()->Attach ("screen_space_reflection", this);
}

void SSRAccumulationContainerRenderSubPass::ClearSettings ()
{
	/*
	 * Detach
	*/

	SettingsManager::Instance ()->Detach ("screen_space_reflection", this);
}
