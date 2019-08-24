#include "VoxelBorderRenderPass.h"

#include "Managers/ShaderManager.h"

#include "Renderer/Pipeline.h"

#include "VoxelVolume.h"

VoxelBorderRenderPass::VoxelBorderRenderPass () :
	_firstTime (true)
{

}

void VoxelBorderRenderPass::Init (const RenderSettings& settings)
{
	ShaderManager::Instance ()->AddComputeShader ("VOXEL_BORDER_PASS_COMPUTE_SHADER",
		"Assets/Shaders/Voxelize/voxelBorderCompute.glsl");
}

RenderVolumeCollection* VoxelBorderRenderPass::Execute (const RenderScene* renderScene, const Camera* camera,
	const RenderSettings& settings, RenderVolumeCollection* rvc)
{
	if (settings.vct_bordering && (_firstTime || settings.vct_continuous_voxelization)) {

		/*
		* Start mipmapping pass
		*/

		StartVoxelBordering ();

		/*
		* Mipmapping pass
		*/

		BorderVoxelVolume (settings, rvc);

		/*
		* End mipmapping pass
		*/

		EndVoxelBordering ();

		_firstTime = false;
	}

	return rvc;
}

void VoxelBorderRenderPass::Clear ()
{

}

void VoxelBorderRenderPass::StartVoxelBordering ()
{
	Pipeline::SetShader (ShaderManager::Instance ()->GetShader ("VOXEL_BORDER_PASS_COMPUTE_SHADER"));
}

void VoxelBorderRenderPass::BorderVoxelVolume (const RenderSettings& settings, RenderVolumeCollection* rvc)
{
	std::size_t dstMipRes = settings.vct_voxels_size;
	Shader* computeShader = ShaderManager::Instance ()->GetShader ("VOXEL_BORDER_PASS_COMPUTE_SHADER");

	VoxelVolume* voxelVolume = (VoxelVolume*) rvc->GetRenderVolume ("VoxelVolume");

	voxelVolume->BindForReading ();

	for (std::size_t mipLevel = 0; mipLevel < voxelVolume->GetVolumeMipmapLevels (); mipLevel++) {

		Pipeline::SendCustomAttributes ("VOXEL_BORDER_PASS_COMPUTE_SHADER", voxelVolume->GetCustomAttributes ());

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
