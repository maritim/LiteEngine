#include "VoxelBorderRenderPass.h"

#include "Resources/Resources.h"
#include "Renderer/RenderSystem.h"

#include "Renderer/Pipeline.h"

#include "VoxelVolume.h"

bool VoxelBorderRenderPass::IsAvailable (const RenderScene* renderScene, const Camera* camera,
	const RenderSettings& settings, const RenderVolumeCollection* rvc) const
{
	/*
	 * Check if voxel border is enabled
	*/

	return settings.vct_bordering;
}

void VoxelBorderRenderPass::Init (const RenderSettings& settings)
{
	/*
	 * Shader for voxel border render pass
	*/

	Resource<Shader> shader = Resources::LoadComputeShader ("Assets/Shaders/Voxelize/voxelBorderCompute.glsl");

	_shaderView = RenderSystem::LoadComputeShader (shader);
}

RenderVolumeCollection* VoxelBorderRenderPass::Execute (const RenderScene* renderScene, const Camera* camera,
	const RenderSettings& settings, RenderVolumeCollection* rvc)
{
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

	return rvc;
}

void VoxelBorderRenderPass::Clear ()
{

}

void VoxelBorderRenderPass::StartVoxelBordering ()
{
	Pipeline::SetShader (_shaderView);
}

void VoxelBorderRenderPass::BorderVoxelVolume (const RenderSettings& settings, RenderVolumeCollection* rvc)
{
	std::size_t dstMipRes = settings.vct_voxels_size;

	VoxelVolume* voxelVolume = (VoxelVolume*) rvc->GetRenderVolume ("VoxelVolume");

	for (std::size_t mipLevel = 0; mipLevel < voxelVolume->GetVolumeMipmapLevels (); mipLevel++) {

		Pipeline::SendCustomAttributes (_shaderView, voxelVolume->GetCustomAttributes ());

		GL::Uniform1i (_shaderView->GetUniformLocation ("SrcMipLevel"), mipLevel);
		GL::Uniform1i (_shaderView->GetUniformLocation ("DstMipRes"), dstMipRes);

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
