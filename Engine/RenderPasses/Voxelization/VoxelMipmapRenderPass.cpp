#include "VoxelMipmapRenderPass.h"

#include "Resources/Resources.h"
#include "Renderer/RenderSystem.h"

#include "Renderer/Pipeline.h"

#include "VoxelVolume.h"

VoxelMipmapRenderPass::VoxelMipmapRenderPass () :
	_firstTime (true)
{

}

void VoxelMipmapRenderPass::Init (const RenderSettings& settings)
{
	/*
	 * Shader for voxel mipmap render pass
	*/

	Resource<Shader> shader = Resources::LoadComputeShader ("Assets/Shaders/Voxelize/voxelMipmapCompute.glsl");

	_shaderView = RenderSystem::LoadComputeShader (shader);
}

RenderVolumeCollection* VoxelMipmapRenderPass::Execute (const RenderScene* renderScene, const Camera* camera,
	const RenderSettings& settings, RenderVolumeCollection* rvc)
{
	if (_firstTime || settings.vct_continuous_voxelization) {

		/*
		* Start mipmapping pass
		*/

		StartVoxelMipmaping ();

		/*
		* Mipmapping pass
		*/

		GenerateMipmaps (settings, rvc);

		/*
		* End mipmapping pass
		*/

		EndVoxelMipmaping ();

		_firstTime = false;
	}

	return rvc;
}

void VoxelMipmapRenderPass::Clear ()
{

}

void VoxelMipmapRenderPass::StartVoxelMipmaping ()
{
	Pipeline::SetShader (_shaderView);
}

void VoxelMipmapRenderPass::GenerateMipmaps (const RenderSettings& settings, RenderVolumeCollection* rvc)
{
	std::size_t dstMipRes = ((int) settings.vct_voxels_size) >> 1;

	VoxelVolume* voxelVolume = (VoxelVolume*) rvc->GetRenderVolume ("VoxelVolume");

	voxelVolume->BindForReading ();

	for (std::size_t mipLevel = 0; mipLevel < voxelVolume->GetVolumeMipmapLevels () - 1; mipLevel++) {

		Pipeline::SendCustomAttributes (_shaderView, voxelVolume->GetCustomAttributes ());

		GL::Uniform1i (_shaderView->GetUniformLocation ("SrcMipLevel"), mipLevel);
		GL::Uniform1i (_shaderView->GetUniformLocation ("DstMipRes"), dstMipRes);

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
