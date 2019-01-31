#include "VoxelRadianceInjectionRenderPass.h"

#include "Managers/ShaderManager.h"

#include "Renderer/Pipeline.h"

#include "Systems/Settings/SettingsManager.h"

VoxelRadianceInjectionRenderPass::VoxelRadianceInjectionRenderPass () :
	_continuousVoxelization (false),
	_firstTime (true)
{

}

void VoxelRadianceInjectionRenderPass::Init ()
{
	ShaderManager::Instance ()->AddComputeShader ("VOXEL_RADIANCE_INJECTION_PASS_COMPUTE_SHADER",
		"Assets/Shaders/Voxelize/voxelRadianceInjectionCompute.glsl");

	/*
	 * Initialize voxel radiance injection settings
	*/

	InitSettings ();
}

RenderVolumeCollection* VoxelRadianceInjectionRenderPass::Execute (const Scene* scene, const Camera* camera, RenderVolumeCollection* rvc)
{
	if (_firstTime || _continuousVoxelization) {

		/*
		* Start radiance injecting pass
		*/

		StartRadianceInjectionPass ();

		/*
		* Radiance Injecting pass
		*/

		RadianceInjectPass (rvc);

		/*
		* End radiance injecting pass
		*/

		EndRadianceInjectionPass ();

		_firstTime = false;
	}

	return rvc;
}

void VoxelRadianceInjectionRenderPass::Notify (Object* sender, const SettingsObserverArgs& args)
{
	std::string name = args.GetName ();

	/*
	 * Update voxel volume size
	*/

	if (name == "vct_voxels_size") {
		_voxelVolumeSize = SettingsManager::Instance ()->GetValue<int> ("vct_voxels_size", _voxelVolumeSize);
	}

	/*
	 * Update continuous voxelization
	*/

	if (name == "vct_continuous_voxelization") {
		_continuousVoxelization = SettingsManager::Instance ()->GetValue<bool> ("vct_continuous_voxelization", _continuousVoxelization);
	}
}

void VoxelRadianceInjectionRenderPass::Clear ()
{
	/*
	 * Clear settings
	*/

	ClearSettings ();
}

void VoxelRadianceInjectionRenderPass::StartRadianceInjectionPass ()
{
	Pipeline::SetShader (ShaderManager::Instance ()->GetShader ("VOXEL_RADIANCE_INJECTION_PASS_COMPUTE_SHADER"));
}

void VoxelRadianceInjectionRenderPass::RadianceInjectPass (RenderVolumeCollection* rvc)
{

	/*
	 * Bind all render volumes
	*/

	// for (RenderVolumeI* renderVolume : *rvc) {
	// 	renderVolume->BindForReading ();
	// }

	/*
	 * Send custom attributes of render volumes to pipeline
	*/

	// for (RenderVolumeI* renderVolume : *rvc) {
	// 	Pipeline::SendCustomAttributes ("", renderVolume->GetCustomAttributes ());
	// }
	
	/*
	 * Bind render volumes for reading
	*/

	rvc->GetRenderVolume ("ShadowMapDirectionalLightVolume")->BindForReading ();

	rvc->GetRenderVolume ("VoxelVolume")->BindForReading ();

	/*
	 * Send custom attributes of render volumes to pipeline
	*/

	Pipeline::SendCustomAttributes ("VOXEL_RADIANCE_INJECTION_PASS_COMPUTE_SHADER",
		rvc->GetRenderVolume ("ShadowMapDirectionalLightVolume")->GetCustomAttributes ());

	Pipeline::SendCustomAttributes ("VOXEL_RADIANCE_INJECTION_PASS_COMPUTE_SHADER",
		rvc->GetRenderVolume ("VoxelVolume")->GetCustomAttributes ());

	/*
	 * Bind voxel volume for writing
	*/

	rvc->GetRenderVolume ("VoxelVolume")->BindForWriting ();

	/*
	 * Inject radiance
	*/

	int numWorkGroups = (int) std::ceil (_voxelVolumeSize / 4.0f);
	GL::DispatchCompute (numWorkGroups, numWorkGroups, numWorkGroups);
}

void VoxelRadianceInjectionRenderPass::EndRadianceInjectionPass ()
{
	/*
	* Make sure writing to image has finished before read
	*/

	GL::MemoryBarrier (GL_TEXTURE_FETCH_BARRIER_BIT | GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
}

void VoxelRadianceInjectionRenderPass::InitSettings ()
{
	/*
	 * Initialize voxel volume size
	*/

	_voxelVolumeSize = SettingsManager::Instance ()->GetValue<int> ("vct_voxels_size", _voxelVolumeSize);

	/*
	 * Initialize continuous voxelization
	*/

	_continuousVoxelization = SettingsManager::Instance ()->GetValue<bool> ("vct_continuous_voxelization", _continuousVoxelization);

	/*
	 * Attach to settings manager
	*/

	SettingsManager::Instance ()->Attach ("vct_voxels_size", this);
	SettingsManager::Instance ()->Attach ("vct_continuous_voxelization", this);
}

void VoxelRadianceInjectionRenderPass::ClearSettings ()
{
	/*
	 * Detach
	*/

	SettingsManager::Instance ()->Detach ("vct_voxels_size", this);
	SettingsManager::Instance ()->Detach ("vct_continuous_voxelization", this);
}
