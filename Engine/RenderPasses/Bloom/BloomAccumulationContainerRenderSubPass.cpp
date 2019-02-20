#include "BloomAccumulationContainerRenderSubPass.h"

#include "Systems/Settings/SettingsManager.h"

#include "Systems/Window/Window.h"

BloomAccumulationContainerRenderSubPass::BloomAccumulationContainerRenderSubPass () :
	PostProcessContainerRenderSubPass (),
	_enabled (false),
	_intensity (0.0f),
	_resolution (0)
{

}

BloomAccumulationContainerRenderSubPass::~BloomAccumulationContainerRenderSubPass ()
{

}

void BloomAccumulationContainerRenderSubPass::Init ()
{
	/*
	 * Initialize bloom settings
	*/

	InitSettings ();

	/*
	 *
	*/

	PostProcessContainerRenderSubPass::Init ();
}

bool BloomAccumulationContainerRenderSubPass::IsAvailable (const Scene* scene, const Camera* camera, const RenderVolumeCollection* rvc) const
{
	/*
	 * Check if bloom is enabled
	*/

	return _enabled;
}

void BloomAccumulationContainerRenderSubPass::Notify (Object* sender, const SettingsObserverArgs& args)
{
	std::string name = args.GetName ();

	/*
	 * Update bloom availability
	*/

	if (name == "bloom") {
		_enabled = SettingsManager::Instance ()->GetValue<bool> ("bloom", _enabled);
	}

	/*
	 * Update bloom intensity
	*/

	if (name == "bloom_intensity") {
		_intensity = SettingsManager::Instance ()->GetValue<float> ("bloom_intensity", _intensity);
	}
}

void BloomAccumulationContainerRenderSubPass::Clear ()
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

std::string BloomAccumulationContainerRenderSubPass::GetPostProcessFragmentShaderPath () const
{
	return "Assets/Shaders/Bloom/bloomAccumulationFragment.glsl";
}

std::string BloomAccumulationContainerRenderSubPass::GetPostProcessVolumeName () const
{
	return "PostProcessMapVolume";
}

glm::ivec2 BloomAccumulationContainerRenderSubPass::GetPostProcessVolumeResolution () const
{
	return glm::ivec2 (Window::GetWidth (), Window::GetHeight ());
}

PostProcessMapVolume* BloomAccumulationContainerRenderSubPass::CreatePostProcessVolume () const
{
	PostProcessMapVolume* volume = new PostProcessMapVolume ();

	return volume;
}

std::vector<PipelineAttribute> BloomAccumulationContainerRenderSubPass::GetCustomAttributes (RenderVolumeCollection* rvc)
{
	/*
	 * Attach post process volume attributes to pipeline
	*/

	std::vector<PipelineAttribute> attributes = PostProcessContainerRenderSubPass::GetCustomAttributes (rvc);

	/*
	 * Attach bloom attributes to pipeline
	*/

	PipelineAttribute bloomIntensity;

	bloomIntensity.type = PipelineAttribute::AttrType::ATTR_1F;

	bloomIntensity.name = "bloomIntensity";

	bloomIntensity.value.x = _intensity;

	attributes.push_back (bloomIntensity);

	return attributes;
}

void BloomAccumulationContainerRenderSubPass::InitSettings ()
{
	/*
	 * Initialize bloom availability
	*/

	_enabled = SettingsManager::Instance ()->GetValue<bool> ("bloom", _enabled);

	/*
	 * Initialize bloom intensity
	*/

	_intensity = SettingsManager::Instance ()->GetValue<float> ("bloom_intensity", _intensity);

	/*
	 * Attach to settings manager
	*/

	SettingsManager::Instance ()->Attach ("bloom", this);
	SettingsManager::Instance ()->Attach ("bloom_intensity", this);
}

void BloomAccumulationContainerRenderSubPass::ClearSettings ()
{
	/*
	 * Detach
	*/

	SettingsManager::Instance ()->Detach ("bloom", this);
	SettingsManager::Instance ()->Detach ("bloom_intensity", this);
}