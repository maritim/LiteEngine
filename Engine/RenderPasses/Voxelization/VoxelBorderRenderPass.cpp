#include "VoxelBorderRenderPass.h"

#include "Managers/ShaderManager.h"

#include "Renderer/Pipeline.h"

#include "Systems/Settings/SettingsManager.h"

#include "VoxelVolume.h"

VoxelBorderRenderPass::VoxelBorderRenderPass () :
	_enabled (false),
	_voxelVolumeSize (0),
	_continuousVoxelization (false),
	_firstTime (true)
{

}

void VoxelBorderRenderPass::Init (const RenderSettings& settings)
{
	ShaderManager::Instance ()->AddComputeShader ("VOXEL_BORDER_PASS_COMPUTE_SHADER",
		"Assets/Shaders/Voxelize/voxelBorderCompute.glsl");

	/*
	 * Initialized voxel border settings
	*/

	InitSettings ();
}

RenderVolumeCollection* VoxelBorderRenderPass::Execute (const RenderScene* renderScene, const Camera* camera,
	const RenderSettings& settings, RenderVolumeCollection* rvc)
{
	if (_enabled && (_firstTime || _continuousVoxelization)) {

		/*
		* Start mipmapping pass
		*/

		StartVoxelBordering ();

		/*
		* Mipmapping pass
		*/

		BorderVoxelVolume (rvc);

		/*
		* End mipmapping pass
		*/

		EndVoxelBordering ();

		_firstTime = false;
	}

	return rvc;
}

void VoxelBorderRenderPass::Notify (Object* sender, const SettingsObserverArgs& args)
{
	std::string name = args.GetName ();

	/*
	 * Update voxel volume bordering availability
	*/

	if (name == "vct_bordering") {
		_enabled = SettingsManager::Instance ()->GetValue<bool> ("vct_bordering", _enabled);
	}

	/*
	 * Update voxel volume size
	*/

	if (name == "vct_voxels_size") {
		_voxelVolumeSize = SettingsManager::Instance ()->GetValue<bool> ("vct_voxels_size", _voxelVolumeSize);
	}

	/*
	 * Update continuous voxelization
	*/

	if (name == "vct_continuous_voxelization") {
		_continuousVoxelization = SettingsManager::Instance ()->GetValue<bool> ("vct_continuous_voxelization", _continuousVoxelization);
	}
}

void VoxelBorderRenderPass::Clear ()
{
	/*
	 * Clear settings
	*/

	ClearSettings ();
}

void VoxelBorderRenderPass::StartVoxelBordering ()
{
	Pipeline::SetShader (ShaderManager::Instance ()->GetShader ("VOXEL_BORDER_PASS_COMPUTE_SHADER"));
}

#define MIPMAP_LEVELS 6

void VoxelBorderRenderPass::BorderVoxelVolume (RenderVolumeCollection* rvc)
{
	std::size_t dstMipRes = _voxelVolumeSize;
	Shader* computeShader = ShaderManager::Instance ()->GetShader ("VOXEL_BORDER_PASS_COMPUTE_SHADER");

	VoxelVolume* voxelVolume = (VoxelVolume*) rvc->GetRenderVolume ("VoxelVolume");

	voxelVolume->BindForReading ();

	for (int mipLevel = 0; mipLevel < MIPMAP_LEVELS; mipLevel++) {

		Pipeline::SendCustomAttributes ("VOXEL_BORDER_PASS_COMPUTE_SHADER",
			rvc->GetRenderVolume ("VoxelVolume")->GetCustomAttributes ());

		GL::Uniform1i (computeShader->GetUniformLocation ("SrcMipLevel"), mipLevel);
		GL::Uniform1i (computeShader->GetUniformLocation ("DstMipRes"), dstMipRes);

		voxelVolume->BindForWriting (mipLevel);

		int numWorkGroups = (int) std::ceil (dstMipRes / 4.0);
		GL::DispatchCompute (numWorkGroups, numWorkGroups, numWorkGroups);

		GL::MemoryBarrier (GL_TEXTURE_FETCH_BARRIER_BIT | GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
		dstMipRes >>= 1;
	}
}

void VoxelBorderRenderPass::EndVoxelBordering ()
{
	/*
	* Make sure writing to image has finished before read
	*/

	GL::MemoryBarrier (GL_TEXTURE_FETCH_BARRIER_BIT | GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
}

void VoxelBorderRenderPass::InitSettings ()
{
	/*
	 * Initialize voxel volume bordering availability
	*/

	_enabled = SettingsManager::Instance ()->GetValue<bool> ("vct_bordering", _enabled);

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

	SettingsManager::Instance ()->Attach ("vct_bordering", this);
	SettingsManager::Instance ()->Attach ("vct_voxels_size", this);
	SettingsManager::Instance ()->Attach ("vct_continuous_voxelization", this);
}

void VoxelBorderRenderPass::ClearSettings ()
{
	/*
	 * Detach
	*/

	SettingsManager::Instance ()->Detach ("vct_bordering", this);
	SettingsManager::Instance ()->Detach ("vct_voxels_size", this);
	SettingsManager::Instance ()->Detach ("vct_continuous_voxelization", this);
}
