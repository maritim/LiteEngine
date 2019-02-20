#include "BrightExtractionContainerRenderSubPass.h"

#include "Systems/Settings/SettingsManager.h"

#include "RenderPasses/Blur/BlurMapVolume.h"

BrightExtractionContainerRenderSubPass::BrightExtractionContainerRenderSubPass () :
	PostProcessContainerRenderSubPass (),
	_enabled (false),
	_threshold (0.0f),
	_resolution (0)
{

}

BrightExtractionContainerRenderSubPass::~BrightExtractionContainerRenderSubPass ()
{

}

void BrightExtractionContainerRenderSubPass::Init ()
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

bool BrightExtractionContainerRenderSubPass::IsAvailable (const Scene* scene, const Camera* camera, const RenderVolumeCollection* rvc) const
{
	/*
	 * Check if bloom is enabled
	*/

	return _enabled;
}

void BrightExtractionContainerRenderSubPass::Notify (Object* sender, const SettingsObserverArgs& args)
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

	/*
	 * Update bloom threshold
	*/

	if (name == "bloom_threshold") {
		_threshold = SettingsManager::Instance ()->GetValue<float> ("bloom_threshold", _threshold);
	}
}

void BrightExtractionContainerRenderSubPass::Clear ()
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

std::string BrightExtractionContainerRenderSubPass::GetPostProcessFragmentShaderPath () const
{
	return "Assets/Shaders/Bloom/brightExtractionFragment.glsl";
}

std::string BrightExtractionContainerRenderSubPass::GetPostProcessVolumeName () const
{
	return "BlurMapVolume";
}

glm::ivec2 BrightExtractionContainerRenderSubPass::GetPostProcessVolumeResolution () const
{
	return _resolution;
}

PostProcessMapVolume* BrightExtractionContainerRenderSubPass::CreatePostProcessVolume () const
{
	BlurMapVolume* blurMapVolume = new BlurMapVolume ();

	return blurMapVolume;
}

std::vector<PipelineAttribute> BrightExtractionContainerRenderSubPass::GetCustomAttributes (RenderVolumeCollection* rvc)
{
	/*
	 * Attach post process volume attributes to pipeline
	*/

	std::vector<PipelineAttribute> attributes = PostProcessContainerRenderSubPass::GetCustomAttributes (rvc);

	/*
	 * Attach bloom attributes to pipeline
	*/

	PipelineAttribute bloomResolution;
	PipelineAttribute bloomThreshold;

	bloomResolution.type = PipelineAttribute::AttrType::ATTR_2F;
	bloomThreshold.type = PipelineAttribute::AttrType::ATTR_1F;

	bloomResolution.name = "bloomResolution";
	bloomThreshold.name = "bloomThreshold";

	bloomResolution.value = glm::vec3 (_resolution, 0.0f);
	bloomThreshold.value.x = _threshold;

	attributes.push_back (bloomResolution);
	attributes.push_back (bloomThreshold);

	return attributes;
}

void BrightExtractionContainerRenderSubPass::InitSettings ()
{
	/*
	 * Initialize bloom availability
	*/

	_enabled = SettingsManager::Instance ()->GetValue<bool> ("bloom", _enabled);

	/*
	 * Initialize bloom resolution
	*/

	_resolution = SettingsManager::Instance ()->GetValue<glm::vec2> ("bloom_resolution", (glm::vec2) _resolution);

	/*
	 * Initialize bloom threshold
	*/

	_threshold = SettingsManager::Instance ()->GetValue<float> ("bloom_threshold", _threshold);

	/*
	 * Attach to settings manager
	*/

	SettingsManager::Instance ()->Attach ("bloom", this);
	SettingsManager::Instance ()->Attach ("bloom_resolution", this);
	SettingsManager::Instance ()->Attach ("bloom_threshold", this);
}

void BrightExtractionContainerRenderSubPass::ClearSettings ()
{
	/*
	 * Detach
	*/

	SettingsManager::Instance ()->Detach ("bloom", this);
	SettingsManager::Instance ()->Detach ("bloom_resolution", this);
	SettingsManager::Instance ()->Detach ("bloom_threshold", this);
}