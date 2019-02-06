#include "SSAOBlurContainerRenderSubPass.h"

#include "Systems/Settings/SettingsManager.h"

#include "SSAOMapVolume.h"

SSAOBlurContainerRenderSubPass::SSAOBlurContainerRenderSubPass () :
	_aoEnabled (false),
	_enabled (false),
	_resolution (0)
{

}

SSAOBlurContainerRenderSubPass::~SSAOBlurContainerRenderSubPass ()
{

}

void SSAOBlurContainerRenderSubPass::Init ()
{
	/*
	 * Initialize screen space ambient occlusion blur settings
	*/

	InitSettings ();

	/*
	 *
	*/

	PostProcessContainerRenderSubPass::Init ();
}

bool SSAOBlurContainerRenderSubPass::IsAvailable (const Scene* scene, const Camera* camera, const RenderVolumeCollection* rvc) const
{
	/*
	 * Check if screen space ambient occlusion blur is enabled
	*/

	return _aoEnabled && _enabled;
}

void SSAOBlurContainerRenderSubPass::Notify (Object* sender, const SettingsObserverArgs& args)
{
	std::string name = args.GetName ();

	/*
	 * Update screen space ambient occlusion availability
	*/

	if (name == "ambient_occlusion") {
		_aoEnabled = SettingsManager::Instance ()->GetValue<bool> ("ambient_occlusion", _aoEnabled);
	}

	/*
	 * Update screen space ambient occlusion blur availability
	*/

	if (name == "ssao_blur") {
		_enabled = SettingsManager::Instance ()->GetValue<bool> ("ssao_blur", _enabled);
	}

	/*
	 * Update screen space ambient occlusion resolution
	*/

	if (name == "ssao_resolution") {
		_resolution = SettingsManager::Instance ()->GetValue<glm::vec2> ("ssao_resolution", (glm::vec2) _resolution);

		/*
		 * Initialize screen space ambient occlusion blur map
		*/

		ReinitPostProcessVolume ();
	}
}

void SSAOBlurContainerRenderSubPass::Clear ()
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

std::string SSAOBlurContainerRenderSubPass::GetPostProcessFragmentShaderPath () const
{
	return "Assets/Shaders/AmbientOcclusion/screenSpaceAmbientOcclusionBlurFragment.glsl";
}

std::string SSAOBlurContainerRenderSubPass::GetPostProcessVolumeName () const
{
	return "SSAOMapVolume";
}

glm::ivec2 SSAOBlurContainerRenderSubPass::GetPostProcessVolumeResolution () const
{
	return _resolution;
}

PostProcessMapVolume* SSAOBlurContainerRenderSubPass::CreatePostProcessVolume () const
{
	SSAOMapVolume* ssaoBlurMapVolume = new SSAOMapVolume ();

	return ssaoBlurMapVolume;
}

std::vector<PipelineAttribute> SSAOBlurContainerRenderSubPass::GetCustomAttributes (RenderVolumeCollection* rvc)
{
	/*
	 * Attach post process volume attributes to pipeline
	*/

	std::vector<PipelineAttribute> attributes = PostProcessContainerRenderSubPass::GetCustomAttributes (rvc);

	/*
	 * Attach screen space ambient occlusion attributes to pipeline
	*/

	PipelineAttribute ssaoResolution;

	ssaoResolution.type = PipelineAttribute::AttrType::ATTR_2F;

	ssaoResolution.name = "ssaoResolution";

	ssaoResolution.value = glm::vec3 (_resolution, 0.0f);

	attributes.push_back (ssaoResolution);

	return attributes;
}

void SSAOBlurContainerRenderSubPass::InitSettings ()
{
	/*
	 * Initialize screen space ambient occlusion availability
	*/

	_aoEnabled = SettingsManager::Instance ()->GetValue<bool> ("ambient_occlusion", _aoEnabled);

	/*
	 * Initialize screen space ambient occlusion blur availability
	*/

	_enabled = SettingsManager::Instance ()->GetValue<bool> ("ssao_blur", _enabled);

	/*
	 * Initialize screen space ambient occlusion resolution
	*/

	_resolution = SettingsManager::Instance ()->GetValue<glm::vec2> ("ssao_resolution", (glm::vec2) _resolution);

	/*
	 * Attach to settings manager
	*/

	SettingsManager::Instance ()->Attach ("ambient_occlusion", this);
	SettingsManager::Instance ()->Attach ("ssao_blur", this);
	SettingsManager::Instance ()->Attach ("ssao_resolution", this);
}

void SSAOBlurContainerRenderSubPass::ClearSettings ()
{
	/*
	 * Detach
	*/

	SettingsManager::Instance ()->Detach ("ambient_occlusion", this);
	SettingsManager::Instance ()->Detach ("ssao_blur", this);
	SettingsManager::Instance ()->Detach ("ssao_resolution", this);
}
