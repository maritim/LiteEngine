#include "SSRContainerRenderSubPass.h"

#include "SSRMapVolume.h"

#include "Systems/Settings/SettingsManager.h"

SSRContainerRenderSubPass::SSRContainerRenderSubPass () :
	_enabled (false),
	_iterations (0),
	_roughness (0.0f),
	_sampleSkip (0.0f),
	_spatialBias (0.0f),
	_resolution (0)
{

}

SSRContainerRenderSubPass::~SSRContainerRenderSubPass ()
{

}

void SSRContainerRenderSubPass::Init ()
{
	/*
	 * Initialize screen space reflection settings
	*/

	InitSettings ();

	/*
	 *
	*/

	PostProcessContainerRenderSubPass::Init ();
}

bool SSRContainerRenderSubPass::IsAvailable (const Scene* scene, const Camera* camera, const RenderVolumeCollection* rvc) const
{
	/*
	 * Check if screen space reflection is enabled
	*/

	return _enabled;
}

void SSRContainerRenderSubPass::Notify (Object* sender, const SettingsObserverArgs& args)
{
	std::string name = args.GetName ();

	/*
	 * Update screen space reflection availability
	*/

	if (name == "screen_space_reflection") {
		_enabled = SettingsManager::Instance ()->GetValue<bool> ("screen_space_reflection", _enabled);
	}

	/*
	 * Update screen space reflection iterations
	*/

	if (name == "ssr_iterations") {
		_iterations = SettingsManager::Instance ()->GetValue<int> ("ssr_iterations", _iterations);
	}

	/*
	 * Update screen space reflection roughness
	*/

	if (name == "ssr_roughness") {
		_roughness = SettingsManager::Instance ()->GetValue<float> ("ssr_roughness", _roughness);
	}

	/*
	 * Update screen space reflection sample skip
	*/

	if (name == "ssr_sample_skip") {
		_sampleSkip = SettingsManager::Instance ()->GetValue<float> ("ssr_sample_skip", _sampleSkip);
	}

	/*
	 * Update screen space reflection spatial bias
	*/

	if (name == "ssr_spatial_bias") {
		_spatialBias = SettingsManager::Instance ()->GetValue<float> ("ssr_spatial_bias", _spatialBias);
	}

	/*
	 * Update screen space reflection resolution
	*/

	if (name == "ssr_resolution") {
		_resolution = SettingsManager::Instance ()->GetValue<glm::vec2> ("ssr_resolution", (glm::vec2) _resolution);

		/*
		 * Initialize screen space reflection map
		*/

		ReinitPostProcessVolume ();
	}
}

void SSRContainerRenderSubPass::Clear ()
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

std::string SSRContainerRenderSubPass::GetPostProcessFragmentShaderPath () const
{
	return "Assets/Shaders/ScreenSpaceReflection/screenSpaceReflectionFragment.glsl";
}

std::string SSRContainerRenderSubPass::GetPostProcessVolumeName () const
{
	return "SSRMapVolume";
}

glm::ivec2 SSRContainerRenderSubPass::GetPostProcessVolumeResolution () const
{
	return _resolution;
}

PostProcessMapVolume* SSRContainerRenderSubPass::CreatePostProcessVolume () const
{
	SSRMapVolume* ssrMapVolume = new SSRMapVolume ();

	return ssrMapVolume;
}

std::vector<PipelineAttribute> SSRContainerRenderSubPass::GetCustomAttributes (RenderVolumeCollection* rvc)
{
	/*
	 * Attach post process volume attributes to pipeline
	*/

	std::vector<PipelineAttribute> attributes = PostProcessContainerRenderSubPass::GetCustomAttributes (rvc);

	/*
	 * Attach screen space ambient occlusion attributes to pipeline
	*/

	PipelineAttribute ssrResolution;
	PipelineAttribute ssrIterations;
	PipelineAttribute ssrRoughness;
	PipelineAttribute ssrSampleSkip;
	PipelineAttribute ssrSpatialBias;

	ssrResolution.type = PipelineAttribute::AttrType::ATTR_2F;
	ssrIterations.type = PipelineAttribute::AttrType::ATTR_1I;
	ssrRoughness.type = PipelineAttribute::AttrType::ATTR_1F;
	ssrSampleSkip.type = PipelineAttribute::AttrType::ATTR_1F;
	ssrSpatialBias.type = PipelineAttribute::AttrType::ATTR_1F;

	ssrResolution.name = "ssrResolution";
	ssrIterations.name = "ssrIterations";
	ssrRoughness.name = "ssrRoughness";
	ssrSampleSkip.name = "ssrSampleSkip";
	ssrSpatialBias.name = "ssrSpatialBias";

	ssrResolution.value = glm::vec3 (_resolution, 0.0f);
	ssrIterations.value.x = _iterations;
	ssrRoughness.value.x = _roughness;
	ssrSampleSkip.value.x = _sampleSkip;
	ssrSpatialBias.value.x = _spatialBias;

	attributes.push_back (ssrResolution);
	attributes.push_back (ssrIterations);
	attributes.push_back (ssrRoughness);
	attributes.push_back (ssrSampleSkip);
	attributes.push_back (ssrSpatialBias);

	return attributes;
}

void SSRContainerRenderSubPass::InitSettings ()
{
	/*
	 * Initialize screen space reflection availability
	*/

	_enabled = SettingsManager::Instance ()->GetValue<bool> ("screen_space_reflection", _enabled);

	/*
	 * Initialize screen space reflection iterations
	*/

	_iterations = SettingsManager::Instance ()->GetValue<int> ("ssr_iterations", _iterations);

	/*
	 * Initialize screen space reflection roughness
	*/

	_roughness = SettingsManager::Instance ()->GetValue<float> ("ssr_roughness", _roughness);

	/*
	 * Initialize screen space reflection sample skip
	*/

	_sampleSkip = SettingsManager::Instance ()->GetValue<float> ("ssr_sample_skip", _sampleSkip);

	/*
	 * Initialize screen space reflection spatial bias
	*/

	_spatialBias = SettingsManager::Instance ()->GetValue<float> ("ssr_spatial_bias", _spatialBias);

	/*
	 * Initialize screen space reflection resolution
	*/

	_resolution = SettingsManager::Instance ()->GetValue<glm::vec2> ("ssr_resolution", (glm::vec2) _resolution);

	/*
	 * Attach to settings manager
	*/

	SettingsManager::Instance ()->Attach ("screen_space_reflection", this);
	SettingsManager::Instance ()->Attach ("ssr_iterations", this);
	SettingsManager::Instance ()->Attach ("ssr_roughness", this);
	SettingsManager::Instance ()->Attach ("ssr_sample_skip", this);
	SettingsManager::Instance ()->Attach ("ssr_spatial_bias", this);
	SettingsManager::Instance ()->Attach ("ssr_resolution", this);
}

void SSRContainerRenderSubPass::ClearSettings ()
{
	/*
	 * Detach
	*/

	SettingsManager::Instance ()->Detach ("screen_space_reflection", this);
	SettingsManager::Instance ()->Detach ("ssr_iterations", this);
	SettingsManager::Instance ()->Detach ("ssr_roughness", this);
	SettingsManager::Instance ()->Detach ("ssr_sample_skip", this);
	SettingsManager::Instance ()->Detach ("ssr_spatial_bias", this);
	SettingsManager::Instance ()->Detach ("ssr_resolution", this);
}
