#include "SSAOBlurContainerRenderSubPass.h"

#include "Systems/Settings/SettingsManager.h"

#include "SSAOMapVolume.h"

SSAOBlurContainerRenderSubPass::SSAOBlurContainerRenderSubPass () :
	_aoEnabled (false),
	_enabled (false)
{

}

SSAOBlurContainerRenderSubPass::~SSAOBlurContainerRenderSubPass ()
{
	ClearSettings ();
}

void SSAOBlurContainerRenderSubPass::Init ()
{
	/*
	 *
	*/

	PostProcessContainerRenderSubPass::Init ();

	/*
	 * Initialize screen space ambient occlusion blur settings
	*/

	InitSettings ();
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
}

std::string SSAOBlurContainerRenderSubPass::GetPostProcessFragmentShaderPath () const
{
	return "Assets/Shaders/AmbientOcclusion/screenSpaceAmbientOcclusionBlurFragment.glsl";
}

std::string SSAOBlurContainerRenderSubPass::GetPostProcessVolumeName () const
{
	return "SSAOMapVolume";
}

PostProcessMapVolume* SSAOBlurContainerRenderSubPass::CreatePostProcessVolume () const
{
	SSAOMapVolume* ssaoBlurMapVolume = new SSAOMapVolume ();

	return ssaoBlurMapVolume;
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
	 * Attach to settings manager
	*/

	SettingsManager::Instance ()->Attach ("ambient_occlusion", this);
	SettingsManager::Instance ()->Attach ("ssao_blur", this);
}

void SSAOBlurContainerRenderSubPass::ClearSettings ()
{
	/*
	 * Detach
	*/

	SettingsManager::Instance ()->Detach ("ambient_occlusion", this);
	SettingsManager::Instance ()->Detach ("ssao_blur", this);
}
