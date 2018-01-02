#include "VoxelMipmapRenderPass.h"

#include "Managers/ShaderManager.h"

#include "Renderer/Pipeline.h"

#include "VoxelVolume.h"

VoxelMipmapRenderPass::~VoxelMipmapRenderPass ()
{

}

void VoxelMipmapRenderPass::Init ()
{
	ShaderManager::Instance ()->AddComputeShader ("VOXEL_MIPMAP_PASS_COMPUTE_SHADER",
		"Assets/Shaders/Voxelize/voxelMipmapCompute.glsl");
}

RenderVolumeCollection* VoxelMipmapRenderPass::Execute (const Scene* scene, const Camera* camera, RenderVolumeCollection* rvc)
{
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

	return rvc;
}

void VoxelMipmapRenderPass::StartVoxelMipmaping ()
{
	Pipeline::SetShader (ShaderManager::Instance ()->GetShader ("VOXEL_MIPMAP_PASS_COMPUTE_SHADER"));
}

void VoxelMipmapRenderPass::GenerateMipmaps (RenderVolumeCollection* rvc)
{
	std::size_t dstMipRes = 512 >> 1;
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

