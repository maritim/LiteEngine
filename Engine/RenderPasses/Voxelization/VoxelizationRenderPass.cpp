#include "VoxelizationRenderPass.h"

#include "Resources/Resources.h"
#include "Renderer/RenderSystem.h"

#include "Renderer/Pipeline.h"

#include "Wrappers/OpenGL/GL.h"

#include "Core/Console/Console.h"

#include "SceneNodes/SceneLayer.h"

VoxelizationRenderPass::VoxelizationRenderPass () :
	_voxelVolume (nullptr),
	_firstTime (true)
{

}

void VoxelizationRenderPass::Init (const RenderSettings& settings)
{
	/*
	 * Initialize voxel volume
	*/

	InitVoxelVolume (settings);

	/*
	 * Shader for static objects
	*/

	Resource<Shader> staticShader = Resources::LoadShader ({
		"Assets/Shaders/Voxelize/voxelizeVertex.glsl",
		"Assets/Shaders/Voxelize/voxelizeFragment.glsl",
		"Assets/Shaders/Voxelize/voxelizeGeometry.glsl"
	});

	_staticShaderView = RenderSystem::LoadShader (staticShader);

	/*
	 * Shader for animated objects
	*/

	Resource<Shader> animationShader = Resources::LoadShader ({
		"Assets/Shaders/Voxelize/voxelizeAnimationVertex.glsl",
		"Assets/Shaders/Voxelize/voxelizeFragment.glsl",
		"Assets/Shaders/Voxelize/voxelizeGeometry.glsl"
	});

	_animationShaderView = RenderSystem::LoadShader (animationShader);
}

RenderVolumeCollection* VoxelizationRenderPass::Execute (const RenderScene* renderScene, const Camera* camera,
	const RenderSettings& settings, RenderVolumeCollection* rvc)
{
	if (_firstTime || settings.vct_continuous_voxelization) {

		/*
		 * Update voxel volume
		*/

		UpdateVoxelVolume (settings);

		/*
		* Voxelization start
		*/

		StartVoxelization ();

		/*
		* Voxelization: voxelize geomtry
		*/

		GeometryVoxelizationPass (renderScene, settings);

		/*
		* Clear opengl state after voxelization
		*/

		EndVoxelization ();

		/*
		 * Send back the collection with voxel volume attached
		*/

		_firstTime = false;
	}

	return rvc->Insert ("VoxelVolume", _voxelVolume);
}

void VoxelizationRenderPass::Clear ()
{
	/*
	 * Clear voxel volume
	*/

	delete _voxelVolume;
}

void VoxelizationRenderPass::StartVoxelization ()
{
	/*
	 * Clear voxel volume
	*/

	_voxelVolume->GetFramebufferView ()->Activate ();

	GL::ClearColor(0, 0, 0, 0);
	GL::Clear(GL_COLOR_BUFFER_BIT);

	GL::BindFramebuffer (GL_FRAMEBUFFER, 0);

	/*
	* Render to window but mask out all color.
	*/

	GL::Disable (GL_CULL_FACE);

	GL::ColorMask (GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
	GL::DepthMask (GL_FALSE);
}

void VoxelizationRenderPass::GeometryVoxelizationPass (const RenderScene* renderScene, const RenderSettings& settings)
{
	/*
	* Update voxel volume based on scene bounding box
	*/

	UpdateVoxelVolumeBoundingBox (renderScene);

	/*
	 * Set viewport
	*/

	GL::Viewport (0, 0, settings.vct_voxels_size, settings.vct_voxels_size);

	/*
	* Bind voxel volume to geometry render pass
	*/

	unsigned int voxelTextureID = _voxelVolume->GetFramebufferView ()->GetTextureView (0)->GetGPUIndex ();
	GL::BindImageTexture (0, voxelTextureID, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA8);

	/*
	* Render geometry
	*/

	for_each_type (RenderObject*, renderObject, *renderScene) {

		/*
		 * Check if it's active
		*/

		if (renderObject->IsActive () == false) {
			continue;
		}

		if (renderObject->GetRenderStage () != RenderStage::RENDER_STAGE_DEFERRED) {
			continue;
		}

		/*
		 * Lock voxelization shader for geomtry rendering
		*/

		LockShader (renderObject->GetSceneLayers ());

		/*
		 * Send voxel volume attributes to pipeline
		*/

		Pipeline::SendCustomAttributes (nullptr, GetCustomAttributes ());

		/*
		 * Voxelize object
		*/

		renderObject->Draw ();
	}
}

void VoxelizationRenderPass::EndVoxelization ()
{
	GL::MemoryBarrier(GL_ALL_BARRIER_BITS);

	/*
	* Clear settings
	*/

	GL::ColorMask (GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
	GL::DepthMask (GL_TRUE);

	/*
	* Unlock voxelization shader
	*/

	Pipeline::UnlockShader ();
}

void VoxelizationRenderPass::LockShader (int sceneLayers)
{
	/*
	 * Unlock last shader
	*/

	Pipeline::UnlockShader ();

	/*
	 * Lock the shader for animations
	*/

	if (sceneLayers & SceneLayer::ANIMATION) {
		Pipeline::LockShader (_animationShaderView);
	}

	/*
	 * Lock general shader for not animated objects
	*/

	if (sceneLayers & (SceneLayer::STATIC | SceneLayer::DYNAMIC)) {
		Pipeline::LockShader (_staticShaderView);
	}
}

void VoxelizationRenderPass::UpdateVoxelVolumeBoundingBox (const RenderScene* renderScene)
{
	auto& volume = renderScene->GetBoundingBox ();

	glm::vec3 minVertex = volume.minVertex;
	glm::vec3 maxVertex = volume.maxVertex;

	_voxelVolume->UpdateBoundingBox (minVertex, maxVertex);
}

void VoxelizationRenderPass::InitVoxelVolume (const RenderSettings& settings)
{
	std::vector<Resource<Texture>> textures;

	for (std::size_t index = 0; index < settings.vct_mipmap_levels; index++) {
		Resource<Texture> texture = Resource<Texture> (new Texture ("voxelVolume"));

		glm::ivec3 size = glm::ivec3 (settings.vct_voxels_size >> index);

		if (index > 0) {
			size.x *= 6;
		}

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
	}

	Resource<Framebuffer> framebuffer = Resource<Framebuffer> (new Framebuffer (textures));

	_voxelVolume = new VoxelVolume (framebuffer);
}

void VoxelizationRenderPass::UpdateVoxelVolume (const RenderSettings& settings)
{
	auto size = _voxelVolume->GetFramebuffer ()->GetTexture (0)->GetSize ();
	std::size_t mipmap_levels = _voxelVolume->GetFramebuffer ()->GetTextureCount ();

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

std::vector<PipelineAttribute> VoxelizationRenderPass::GetCustomAttributes ()
{
	std::vector<PipelineAttribute> attributes;

	PipelineAttribute minVertex;
	PipelineAttribute maxVertex;
	PipelineAttribute volumeSizeAttribute;
	PipelineAttribute volumeMipmapLevels;

	minVertex.type = PipelineAttribute::AttrType::ATTR_3F;
	maxVertex.type = PipelineAttribute::AttrType::ATTR_3F;
	volumeSizeAttribute.type = PipelineAttribute::AttrType::ATTR_3I;
	volumeMipmapLevels.type = PipelineAttribute::AttrType::ATTR_1I;

	minVertex.name = "minVertex";
	maxVertex.name = "maxVertex";
	volumeSizeAttribute.name = "volumeSize";
	volumeMipmapLevels.name = "volumeMipmapLevels";

	minVertex.value = _voxelVolume->GetMinVertex ();
	maxVertex.value = _voxelVolume->GetMaxVertex ();
	volumeSizeAttribute.value = glm::vec3 ((float) _voxelVolume->GetFramebuffer ()->GetTexture (0)->GetSize ().width);
	volumeMipmapLevels.value.x = _voxelVolume->GetFramebuffer ()->GetTextureCount ();

	attributes.push_back (minVertex);
	attributes.push_back (maxVertex);
	attributes.push_back (volumeSizeAttribute);
	attributes.push_back (volumeMipmapLevels);

	return attributes;
}
