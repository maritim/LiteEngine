#include "VoxelAlphaUpdateRenderPass.h"

#include "Managers/ShaderManager.h"

#include "Renderer/Pipeline.h"

#include "VoxelVolume.h"

VoxelAlphaUpdateRenderPass::VoxelAlphaUpdateRenderPass () :
	_shaderName ("VOXEL_ALPHA_UPDATE_PASS_COMPUTE_SHADER"),
	_firstTime (true)
{

}

void VoxelAlphaUpdateRenderPass::Init (const RenderSettings& settings)
{
	ShaderManager::Instance ()->AddComputeShader (_shaderName,
		"Assets/Shaders/Voxelize/voxelAlphaUpdateCompute.glsl");
}

RenderVolumeCollection* VoxelAlphaUpdateRenderPass::Execute (const RenderScene* renderScene, const Camera* camera,
	const RenderSettings& settings, RenderVolumeCollection* rvc)
{
	if (_firstTime || settings.vct_continuous_voxelization) {

		/*
		* Start voxel alpha update pass
		*/

		StartVoxelAlphaUpdate ();

		/*
		* Update voxel alpha pass
		*/

		UpdateVoxelAlpha (settings, rvc);

		/*
		* End mipmapping pass
		*/

		EndVoxelAlphaUpdate ();

		_firstTime = false;
	}

	return rvc;
}

void VoxelAlphaUpdateRenderPass::Clear ()
{

}

void VoxelAlphaUpdateRenderPass::StartVoxelAlphaUpdate ()
{
	Pipeline::SetShader (ShaderManager::Instance ()->GetShader (_shaderName));
}

void VoxelAlphaUpdateRenderPass::UpdateVoxelAlpha (const RenderSettings& settings, RenderVolumeCollection* rvc)
{
	VoxelVolume* voxelVolume = (VoxelVolume*) rvc->GetRenderVolume ("VoxelVolume");

	voxelVolume->BindForWriting (0, true);

	int numWorkGroups = (int) std::ceil (settings.vct_voxels_size / 4.0);
	GL::DispatchCompute (numWorkGroups, numWorkGroups, numWorkGroups);

	GL::MemoryBarrier (GL_TEXTURE_FETCH_BARRIER_BIT | GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
}

void VoxelAlphaUpdateRenderPass::EndVoxelAlphaUpdate ()
{
	/*
	* Make sure writing to image has finished before read
	*/

	GL::MemoryBarrier (GL_TEXTURE_FETCH_BARRIER_BIT | GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
}
