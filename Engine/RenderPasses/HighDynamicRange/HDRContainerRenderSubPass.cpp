#include "HDRContainerRenderSubPass.h"

#include "Systems/Window/Window.h"
#include "Systems/Settings/SettingsManager.h"

HDRContainerRenderSubPass::HDRContainerRenderSubPass () :
	PostProcessContainerRenderSubPass (),
	_enabled (false),
	_exposure (0.0f)
{

}

HDRContainerRenderSubPass::~HDRContainerRenderSubPass ()
{

}

void HDRContainerRenderSubPass::Init ()
{
	/*
	 * Initialize high dynamic range settings
	*/

	InitSettings ();

	/*
	 *
	*/

	PostProcessContainerRenderSubPass::Init ();
}

bool HDRContainerRenderSubPass::IsAvailable (const Scene* scene, const Camera* camera, const RenderVolumeCollection* rvc) const
{
	/*
	 * Check if high dynamic range is enabled
	*/

	return _enabled;
}

void HDRContainerRenderSubPass::Notify (Object* sender, const SettingsObserverArgs& args)
{
	std::string name = args.GetName ();

	/*
	 * Update high dynamic range availability
	*/

	if (name == "high_dynamic_range") {
		_enabled = SettingsManager::Instance ()->GetValue<bool> ("high_dynamic_range", _enabled);
	}

	/*
	 * Update high dynamic range exposure
	*/

	if (name == "hdr_exposure") {
		_exposure = SettingsManager::Instance ()->GetValue<float> ("hdr_exposure", _exposure);
	}
}

void HDRContainerRenderSubPass::Clear ()
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

std::string HDRContainerRenderSubPass::GetPostProcessFragmentShaderPath () const
{
	return "Assets/Shaders/HighDynamicRange/highDynamicRangeFragment.glsl";
}

std::string HDRContainerRenderSubPass::GetPostProcessVolumeName () const
{
	return "PostProcessMapVolume";
}

glm::ivec2 HDRContainerRenderSubPass::GetPostProcessVolumeResolution () const
{
	return glm::ivec2 (Window::GetWidth (), Window::GetHeight ());
}

PostProcessMapVolume* HDRContainerRenderSubPass::CreatePostProcessVolume () const
{
	PostProcessMapVolume* volume = new PostProcessMapVolume ();

	return volume;
}

std::vector<PipelineAttribute> HDRContainerRenderSubPass::GetCustomAttributes (RenderVolumeCollection* rvc)
{
	/*
	 * Attach post process volume attributes to pipeline
	*/

	std::vector<PipelineAttribute> attributes = PostProcessContainerRenderSubPass::GetCustomAttributes (rvc);

	/*
	 * Attach high dynamic range attributes to pipeline
	*/

	PipelineAttribute exposure;

	exposure.type = PipelineAttribute::AttrType::ATTR_1F;

	exposure.name = "exposure";

	exposure.value.x = _exposure;

	attributes.push_back (exposure);

	return attributes;
}

void HDRContainerRenderSubPass::InitSettings ()
{
	/*
	 * Initialize high dynamic range availability
	*/

	_enabled = SettingsManager::Instance ()->GetValue<bool> ("high_dynamic_range", _enabled);

	/*
	 * Initialize high dynamic range exposure
	*/

	_exposure = SettingsManager::Instance ()->GetValue<float> ("hdr_exposure", _exposure);

	/*
	 * Attach to settings manager
	*/

	SettingsManager::Instance ()->Attach ("high_dynamic_range", this);
	SettingsManager::Instance ()->Attach ("hdr_exposure", this);
}

void HDRContainerRenderSubPass::ClearSettings ()
{
	/*
	 * Detach
	*/

	SettingsManager::Instance ()->Detach ("high_dynamic_range", this);
	SettingsManager::Instance ()->Detach ("hdr_exposure", this);
}