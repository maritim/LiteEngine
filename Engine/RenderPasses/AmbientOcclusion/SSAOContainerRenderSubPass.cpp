#include "SSAOContainerRenderSubPass.h"

#include "Systems/Settings/SettingsManager.h"

#include "SSAOMapVolume.h"

SSAOContainerRenderSubPass::SSAOContainerRenderSubPass () :
	_enabled (false),
	_resolution (0),
	_radius (0.0f),
	_bias (0.0f)
{

}

SSAOContainerRenderSubPass::~SSAOContainerRenderSubPass ()
{

}

void SSAOContainerRenderSubPass::Init ()
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

bool SSAOContainerRenderSubPass::IsAvailable (const Scene* scene, const Camera* camera, const RenderVolumeCollection* rvc) const
{
	/*
	 * Check if screen space ambient occlusion is enabled
	*/

	return _enabled;
}

void SSAOContainerRenderSubPass::Notify (Object* sender, const SettingsObserverArgs& args)
{
	std::string name = args.GetName ();

	/*
	 * Update screen space ambient occlusion availability
	*/

	if (name == "ambient_occlusion") {
		_enabled = SettingsManager::Instance ()->GetValue<bool> ("ambient_occlusion", _enabled);
	}

	/*
	 * Update screen space ambient occlusion resolution
	*/

	if (name == "ssao_resolution") {
		_resolution = SettingsManager::Instance ()->GetValue<glm::vec2> ("ssao_resolution", (glm::vec2) _resolution);

		/*
		 * Initialize screen space ambient occlusion map
		*/

		ReinitPostProcessVolume ();
	}

	/*
	 * Update screen space ambient occlusion radius
	*/

	if (name == "ssao_radius") {
		_radius = SettingsManager::Instance ()->GetValue<float> ("ssao_radius", _radius);
	}

	/*
	 * Update screen space ambient occlusion bias
	*/

	if (name == "ssao_bias") {
		_bias = SettingsManager::Instance ()->GetValue<float> ("ssao_bias", _bias);
	}
}

void SSAOContainerRenderSubPass::Clear ()
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

std::string SSAOContainerRenderSubPass::GetPostProcessFragmentShaderPath () const
{
	return "Assets/Shaders/AmbientOcclusion/screenSpaceAmbientOcclusionFragment.glsl";
}

std::string SSAOContainerRenderSubPass::GetPostProcessVolumeName () const
{
	return "SSAOMapVolume";
}

glm::ivec2 SSAOContainerRenderSubPass::GetPostProcessVolumeResolution () const
{
	return _resolution;
}

PostProcessMapVolume* SSAOContainerRenderSubPass::CreatePostProcessVolume () const
{
	SSAOMapVolume* ssaoMapVolume = new SSAOMapVolume ();

	return ssaoMapVolume;
}

std::vector<PipelineAttribute> SSAOContainerRenderSubPass::GetCustomAttributes (RenderVolumeCollection* rvc)
{
	/*
	 * Attach post process volume attributes to pipeline
	*/

	std::vector<PipelineAttribute> attributes = PostProcessContainerRenderSubPass::GetCustomAttributes (rvc);

	/*
	 * Attach screen space ambient occlusion attributes to pipeline
	*/

	PipelineAttribute ssaoResolution;
	PipelineAttribute ssaoRadius;
	PipelineAttribute ssaoBias;

	ssaoResolution.type = PipelineAttribute::AttrType::ATTR_2F;
	ssaoRadius.type = PipelineAttribute::AttrType::ATTR_1F;
	ssaoBias.type = PipelineAttribute::AttrType::ATTR_1F;

	ssaoResolution.name = "ssaoResolution";
	ssaoRadius.name = "ssaoRadius";
	ssaoBias.name = "ssaoBias";

	ssaoResolution.value = glm::vec3 (_resolution, 0.0f);
	ssaoRadius.value.x = _radius;
	ssaoBias.value.x = _bias;

	attributes.push_back (ssaoResolution);
	attributes.push_back (ssaoRadius);
	attributes.push_back (ssaoBias);

	return attributes;
}

void SSAOContainerRenderSubPass::InitSettings ()
{
	/*
	 * Initialize screen space ambient occlusion availability
	*/

	_enabled = SettingsManager::Instance ()->GetValue<bool> ("ambient_occlusion", _enabled);

	/*
	 * Initialize screen space ambient occlusion resolution
	*/

	_resolution = SettingsManager::Instance ()->GetValue<glm::vec2> ("ssao_resolution", (glm::vec2) _resolution);

	/*
	 * Initialize screen space ambient occlusion radius
	*/

	_radius = SettingsManager::Instance ()->GetValue<float> ("ssao_radius", _radius);

	/*
	 * Initialize screen space ambient occlusion bias
	*/

	_bias = SettingsManager::Instance ()->GetValue<float> ("ssao_bias", _bias);

	/*
	 * Attach to settings manager
	*/

	SettingsManager::Instance ()->Attach ("ambient_occlusion", this);
	SettingsManager::Instance ()->Attach ("ssao_resolution", this);
	SettingsManager::Instance ()->Attach ("ssao_radius", this);
	SettingsManager::Instance ()->Attach ("ssao_bias", this);
}

void SSAOContainerRenderSubPass::ClearSettings ()
{
	/*
	 * Detach
	*/

	SettingsManager::Instance ()->Detach ("ambient_occlusion", this);
	SettingsManager::Instance ()->Detach ("ssao_resolution", this);
	SettingsManager::Instance ()->Detach ("ssao_radius", this);
	SettingsManager::Instance ()->Detach ("ssao_bias", this);
}
