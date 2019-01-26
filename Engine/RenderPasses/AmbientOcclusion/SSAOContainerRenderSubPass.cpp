#include "SSAOContainerRenderSubPass.h"

#include "Systems/Settings/SettingsManager.h"

#include "SSAOMapVolume.h"

SSAOContainerRenderSubPass::SSAOContainerRenderSubPass () :
	_enabled (false),
	_radius (0.0f),
	_bias (0.0f)
{

}

SSAOContainerRenderSubPass::~SSAOContainerRenderSubPass ()
{
	ClearSettings ();
}

void SSAOContainerRenderSubPass::Init ()
{
	/*
	 *
	*/

	PostProcessContainerRenderSubPass::Init ();

	/*
	 * Initialize screen space ambient occlusion settings
	*/

	InitSettings ();
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

std::string SSAOContainerRenderSubPass::GetPostProcessFragmentShaderPath () const
{
	return "Assets/Shaders/AmbientOcclusion/screenSpaceAmbientOcclusionFragment.glsl";
}

std::string SSAOContainerRenderSubPass::GetPostProcessVolumeName () const
{
	return "SSAOMapVolume";
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

	PipelineAttribute ssaoRadius;
	PipelineAttribute ssaoBias;

	ssaoRadius.type = PipelineAttribute::AttrType::ATTR_1F;
	ssaoBias.type = PipelineAttribute::AttrType::ATTR_1F;

	ssaoRadius.name = "ssaoRadius";
	ssaoBias.name = "ssaoBias";

	ssaoRadius.value.x = _radius;
	ssaoBias.value.x = _bias;

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
	SettingsManager::Instance ()->Attach ("ssao_radius", this);
	SettingsManager::Instance ()->Attach ("ssao_bias", this);
}

void SSAOContainerRenderSubPass::ClearSettings ()
{
	/*
	 * Detach
	*/

	SettingsManager::Instance ()->Detach ("ambient_occlusion", this);
	SettingsManager::Instance ()->Detach ("ssao_radius", this);
	SettingsManager::Instance ()->Detach ("ssao_bias", this);
}
