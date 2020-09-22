#include "VoxelMipmapRenderPass.h"

#include "Resources/Resources.h"
#include "Renderer/RenderSystem.h"

#include "Renderer/Pipeline.h"

#include "VoxelVolume.h"

bool VoxelMipmapRenderPass::IsAvailable (const RenderScene* renderScene, const Camera* camera,
	const RenderSettings& settings, const RenderVolumeCollection* rvc) const
{
	/*
	 * Always allow voxel mipmap
	*/

	return true;
}

void VoxelMipmapRenderPass::Init (const RenderSettings& settings)
{
	/*
	 * Anisotropic mipmap
	*/

	Resource<Shader> shader = Resources::LoadComputeShader (
		"Assets/Shaders/Voxelize/voxelAnisotropicMipmapCompute.glsl"
	);

	_anisotropicShaderView = RenderSystem::LoadComputeShader (shader);

	/*
	 * Shader for voxel mipmap render pass
	*/

	shader = Resources::LoadComputeShader (
		"Assets/Shaders/Voxelize/voxelMipmapCompute.glsl"
	);

	_shaderView = RenderSystem::LoadComputeShader (shader);
}

RenderVolumeCollection* VoxelMipmapRenderPass::Execute (const RenderScene* renderScene, const Camera* camera,
	const RenderSettings& settings, RenderVolumeCollection* rvc)
{
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

	for (std::size_t mipLevel = 0; mipLevel < voxelVolume->GetMipmapLevels () - 1; mipLevel++) {

		if (mipLevel == 0) {
			Pipeline::SetShader (_anisotropicShaderView);

			Pipeline::SendCustomAttributes (_anisotropicShaderView, GetCustomAttributes (rvc, 0));

			GL::Uniform1i (_anisotropicShaderView->GetUniformLocation ("DstMipRes"), dstMipRes);
		} else {
			Pipeline::SetShader (_shaderView);

			Pipeline::SendCustomAttributes (_shaderView, GetCustomAttributes (rvc, 1));

			GL::Uniform1i (_shaderView->GetUniformLocation ("DstMipRes"), dstMipRes);
		}

		GL::Uniform1i (_shaderView->GetUniformLocation ("srcMipLevel"), mipLevel == 0 ? 0 : mipLevel - 1);

		unsigned int voxelTextureID = voxelVolume->GetFramebufferView ()->GetTextureView (1)->GetGPUIndex ();
		GL::BindImageTexture (0, voxelTextureID, mipLevel, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA8);

		int numWorkGroups = (int) std::ceil (dstMipRes / 4.0);

		if (mipLevel == 0) {
			GL::DispatchCompute (numWorkGroups, numWorkGroups, numWorkGroups);
		} else {
			GL::DispatchCompute (numWorkGroups * 6, numWorkGroups, numWorkGroups);
		}

		GL::MemoryBarrier (GL_TEXTURE_FETCH_BARRIER_BIT | GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
		dstMipRes >>= 1;

		Pipeline::UnlockShader ();
	}
}

void VoxelMipmapRenderPass::EndVoxelMipmaping ()
{
	/*
	* Make sure writing to image has finished before read
	*/

	GL::MemoryBarrier (GL_TEXTURE_FETCH_BARRIER_BIT | GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
}

std::vector<PipelineAttribute> VoxelMipmapRenderPass::GetCustomAttributes (RenderVolumeCollection* rvc, std::size_t mipLevel)
{
	VoxelVolume* voxelVolume = (VoxelVolume*) rvc->GetRenderVolume ("VoxelVolume");

	std::vector<PipelineAttribute> attributes;

	PipelineAttribute srcTextureMip;

	srcTextureMip.type = PipelineAttribute::AttrType::ATTR_TEXTURE_3D;

	srcTextureMip.name = "srcTextureMip";

	srcTextureMip.value.x = voxelVolume->GetFramebufferView ()->GetTextureView (mipLevel)->GetGPUIndex ();

	attributes.push_back (srcTextureMip);

	return attributes;
}
