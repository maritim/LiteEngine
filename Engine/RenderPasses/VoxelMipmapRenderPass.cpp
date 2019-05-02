#include "VoxelMipmapRenderPass.h"

#include "Managers/ShaderManager.h"

#include "Renderer/Pipeline.h"

#include "Systems/Settings/SettingsManager.h"

#include "VoxelVolume.h"

VoxelMipmapRenderPass::VoxelMipmapRenderPass () :
	_voxelVolumeSize (0),
	_continuousVoxelization (false),
	_firstTime (true)
{

}

void VoxelMipmapRenderPass::Init (const RenderSettings& settings)
{
	ShaderManager::Instance ()->AddComputeShader ("VOXEL_MIPMAP_PASS_COMPUTE_SHADER",
		"Assets/Shaders/Voxelize/voxelMipmapCompute.glsl");

	/*
	 * Initialize voxel mipmap settings
	*/

	InitSettings ();
}

RenderVolumeCollection* VoxelMipmapRenderPass::Execute (const Scene* scene, const Camera* camera,
	const RenderSettings& settings, RenderVolumeCollection* rvc)
{
	if (_firstTime || _continuousVoxelization) {

		/*
		* Start mipmapping pass
		*/

		StartVoxelMipmaping ();

		/*
		* Mipmapping pass
		*/

		GenerateMipmaps (rvc);

		/*
		* End mipmapping pass
		*/

		EndVoxelMipmaping ();

		_firstTime = false;
	}

	return rvc;
}

void VoxelMipmapRenderPass::Notify (Object* sender, const SettingsObserverArgs& args)
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

void VoxelMipmapRenderPass::Clear ()
{
	/*
	 * Clear settings
	*/

	ClearSettings ();
}

void VoxelMipmapRenderPass::StartVoxelMipmaping ()
{
	Pipeline::SetShader (ShaderManager::Instance ()->GetShader ("VOXEL_MIPMAP_PASS_COMPUTE_SHADER"));
}

void VoxelMipmapRenderPass::GenerateMipmaps (RenderVolumeCollection* rvc)
{
	std::size_t dstMipRes = _voxelVolumeSize >> 1;
	Shader* computeShader = ShaderManager::Instance ()->GetShader ("VOXEL_MIPMAP_PASS_COMPUTE_SHADER");

	VoxelVolume* voxelVolume = (VoxelVolume*) rvc->GetRenderVolume ("VoxelVolume");

	voxelVolume->BindForReading ();

	for (int mipLevel = 0; mipLevel < MIPMAP_LEVELS - 1; mipLevel++) {

		Pipeline::SendCustomAttributes ("VOXEL_MIPMAP_PASS_COMPUTE_SHADER",
			rvc->GetRenderVolume ("VoxelVolume")->GetCustomAttributes ());

		GL::Uniform1i (computeShader->GetUniformLocation ("SrcMipLevel"), mipLevel);
		GL::Uniform1i (computeShader->GetUniformLocation ("DstMipRes"), dstMipRes);

		voxelVolume->BindForWriting (mipLevel + 1);

		int numWorkGroups = (int) std::ceil (dstMipRes / 4.0);
		GL::DispatchCompute (numWorkGroups, numWorkGroups, numWorkGroups);

		GL::MemoryBarrier (GL_TEXTURE_FETCH_BARRIER_BIT | GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
		dstMipRes >>= 1;
	}
}

void VoxelMipmapRenderPass::EndVoxelMipmaping ()
{
	/*
	* Make sure writing to image has finished before read
	*/

	GL::MemoryBarrier (GL_TEXTURE_FETCH_BARRIER_BIT | GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
}

void VoxelMipmapRenderPass::InitSettings ()
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

void VoxelMipmapRenderPass::ClearSettings ()
{
	/*
	 * Detach
	*/

	SettingsManager::Instance ()->Detach ("vct_voxels_size", this);
	SettingsManager::Instance ()->Detach ("vct_continuous_voxelization", this);
}
