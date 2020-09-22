#include "VoxelGenerationRenderPass.h"

#include "Resources/Resources.h"
#include "Renderer/RenderSystem.h"

#include "Renderer/Pipeline.h"

#include "Wrappers/OpenGL/GL.h"

#include "Core/Console/Console.h"

#include "SceneNodes/SceneLayer.h"

VoxelGenerationRenderPass::VoxelGenerationRenderPass () :
	_voxelVolume (nullptr)
{

}

bool VoxelGenerationRenderPass::IsAvailable (const RenderScene* renderScene, const Camera* camera,
	const RenderSettings& settings, const RenderVolumeCollection* rvc) const
{
	/*
	 * Always allow voxelization pass
	*/

	return true;
}

void VoxelGenerationRenderPass::Init (const RenderSettings& settings)
{
	/*
	 * Initialize voxel volume
	*/

	InitVoxelVolume (settings);
}

RenderVolumeCollection* VoxelGenerationRenderPass::Execute (const RenderScene* renderScene, const Camera* camera,
	const RenderSettings& settings, RenderVolumeCollection* rvc)
{
	/*
	 * Update voxel volume
	*/

	UpdateVoxelVolume (settings);

	/*
	* Update voxel volume based on scene bounding box
	*/

	UpdateVoxelVolumeBoundingBox (renderScene);

	return rvc->Insert ("VoxelVolume", _voxelVolume);
}

void VoxelGenerationRenderPass::Clear ()
{
	/*
	 * Clear voxel volume
	*/

	delete _voxelVolume;
}

void VoxelGenerationRenderPass::UpdateVoxelVolumeBoundingBox (const RenderScene* renderScene)
{
	auto& volume = renderScene->GetBoundingBox ();

	glm::vec3 minVertex = volume.minVertex;
	glm::vec3 maxVertex = volume.maxVertex;

	_voxelVolume->UpdateBoundingBox (minVertex, maxVertex);
}

void VoxelGenerationRenderPass::InitVoxelVolume (const RenderSettings& settings)
{
	std::vector<Resource<Texture>> textures;

	/*
	 * Highest resolution
	*/

	Resource<Texture> texture = Resource<Texture> (new Texture ("voxelTexture"));

	glm::ivec3 size = glm::ivec3 (settings.vct_voxels_size);

	texture->SetType (TEXTURE_TYPE::TEXTURE_3D);
	texture->SetSize (Size (size.x, size.y, size.z));
	texture->SetMipmapGeneration (false);
	texture->SetSizedInternalFormat (TEXTURE_SIZED_INTERNAL_FORMAT::FORMAT_RGBA8);
	texture->SetInternalFormat (TEXTURE_INTERNAL_FORMAT::FORMAT_RGBA);
	texture->SetChannelType (TEXTURE_CHANNEL_TYPE::CHANNEL_UNSIGNED_BYTE);
	texture->SetWrapMode (TEXTURE_WRAP_MODE::WRAP_CLAMP_BORDER);
	texture->SetMinFilter (TEXTURE_FILTER_MODE::FILTER_LINEAR);
	texture->SetMagFilter (TEXTURE_FILTER_MODE::FILTER_LINEAR);
	texture->SetAnisotropicFiltering (false);
	texture->SetBorderColor (Color::Black);

	textures.push_back (texture);

	/*
	 * Mipmap texture
	*/

	texture = Resource<Texture> (new Texture ("voxelMipmapTexture"));

	size = glm::ivec3 (settings.vct_voxels_size >> 1);
	size.x *= 6;

	texture->SetType (TEXTURE_TYPE::TEXTURE_3D);
	texture->SetSize (Size (size.x, size.y, size.z));
	texture->SetSizedInternalFormat (TEXTURE_SIZED_INTERNAL_FORMAT::FORMAT_RGBA8);
	texture->SetInternalFormat (TEXTURE_INTERNAL_FORMAT::FORMAT_RGBA);
	texture->SetChannelType (TEXTURE_CHANNEL_TYPE::CHANNEL_UNSIGNED_BYTE);
	texture->SetWrapMode (TEXTURE_WRAP_MODE::WRAP_CLAMP_BORDER);
	texture->SetMinFilter (TEXTURE_FILTER_MODE::FILTER_LINEAR_MIPMAP_LINEAR);
	texture->SetMagFilter (TEXTURE_FILTER_MODE::FILTER_LINEAR);
	texture->SetAnisotropicFiltering (false);
	texture->SetBorderColor (Color::Black);

	textures.push_back (texture);

	Resource<Framebuffer> framebuffer = Resource<Framebuffer> (new Framebuffer (textures));

	_voxelVolume = new VoxelVolume (framebuffer, settings.vct_mipmap_levels);
}

void VoxelGenerationRenderPass::UpdateVoxelVolume (const RenderSettings& settings)
{
	auto size = _voxelVolume->GetFramebuffer ()->GetTexture (0)->GetSize ();
	std::size_t mipmap_levels = _voxelVolume->GetMipmapLevels ();

	if (size.width != settings.vct_voxels_size ||
		mipmap_levels != settings.vct_mipmap_levels) {

		/*
		 * Clear voxel volume
		*/

		delete _voxelVolume;

		/*
		 * Initialize voxel volume
		*/

		InitVoxelVolume (settings);
	}
}
