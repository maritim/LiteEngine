#include "SSRAccumulationContainerRenderSubPass.h"

#include "Systems/Settings/SettingsManager.h"

#include "Systems/Window/Window.h"

SSRAccumulationContainerRenderSubPass::SSRAccumulationContainerRenderSubPass () :
	_enabled (false),
	_intensity (0.0f)
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
	 * Update screen space reflection availability
	*/

	if (name == "screen_space_reflection") {
		_enabled = SettingsManager::Instance ()->GetValue<bool> ("screen_space_reflection", _enabled);
	}

	/*
	 * Update screen space reflection intensity
	*/

	if (name == "ssr_intensity") {
		_intensity = SettingsManager::Instance ()->GetValue<float> ("ssr_intensity", _intensity);
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

std::vector<PipelineAttribute> SSRAccumulationContainerRenderSubPass::GetCustomAttributes (RenderVolumeCollection* rvc)
{
	/*
	 * Attach post process volume attributes to pipeline
	*/

	std::vector<PipelineAttribute> attributes = PostProcessContainerRenderSubPass::GetCustomAttributes (rvc);

	/*
	 * Attach screen space ambient occlusion attributes to pipeline
	*/

	PipelineAttribute ssrIntensity;

	ssrIntensity.type = PipelineAttribute::AttrType::ATTR_1F;

	ssrIntensity.name = "ssrIntensity";

	ssrIntensity.value.x = _intensity;

	attributes.push_back (ssrIntensity);

	return attributes;
}

void SSRAccumulationContainerRenderSubPass::InitSettings ()
{
	/*
	 * Initialize screen space reflection availability
	*/

	_enabled = SettingsManager::Instance ()->GetValue<bool> ("screen_space_reflection", _enabled);

	/*
	 * Initialize screen space reflection intensity
	*/

	_intensity = SettingsManager::Instance ()->GetValue<float> ("ssr_intensity", _intensity);

	/*
	 * Attach to settings manager
	*/

	SettingsManager::Instance ()->Attach ("screen_space_reflection", this);
	SettingsManager::Instance ()->Attach ("ssr_intensity", this);
}

void SSRAccumulationContainerRenderSubPass::ClearSettings ()
{
	/*
	 * Detach
	*/

	SettingsManager::Instance ()->Detach ("screen_space_reflection", this);
	SettingsManager::Instance ()->Detach ("ssr_intensity", this);
}
