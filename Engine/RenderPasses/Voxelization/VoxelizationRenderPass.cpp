#include "VoxelizationRenderPass.h"

#include "Resources/Resources.h"
#include "Renderer/RenderSystem.h"

#include "Renderer/Pipeline.h"

#include "Wrappers/OpenGL/GL.h"

#include "Core/Console/Console.h"

#include "SceneNodes/SceneLayer.h"

VoxelizationRenderPass::VoxelizationRenderPass () :
	_voxelVolume (new VoxelVolume ()),
	_firstTime (true)
{

}

VoxelizationRenderPass::~VoxelizationRenderPass ()
{
	delete _voxelVolume;
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
		"Assets/Shaders/Voxelize/voxelizeVertex.glsl",
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

	_voxelVolume->Clear ();
}

void VoxelizationRenderPass::StartVoxelization ()
{
	/*
	 * Clear voxel volume
	*/

	_voxelVolume->ClearVoxels ();

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

	_voxelVolume->BindForWriting ();

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

		Pipeline::SendCustomAttributes (nullptr, _voxelVolume->GetCustomAttributes ());

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
	if (!_voxelVolume->Init (settings.vct_voxels_size)) {
		Console::LogError (std::string () +
			"Voxel texture cannot be initialized!" +
			" It is not possible to continue the process. End now!");
		exit (VOXEL_TEXTURE_NOT_INIT);
	}

	/*
	 * Set voxel volume mipmap levels
	*/

	_voxelVolume->SetVolumeMipmapLevels (settings.vct_mipmap_levels);
}

void VoxelizationRenderPass::UpdateVoxelVolume (const RenderSettings& settings)
{
	if (_voxelVolume->GetVolumeSize () != settings.vct_voxels_size) {

		/*
		 * Clear voxel volume
		*/

		_voxelVolume->Clear ();

		/*
		 * Initialize voxel volume
		*/

		InitVoxelVolume (settings);
	}

	if (_voxelVolume->GetVolumeMipmapLevels () != settings.vct_mipmap_levels) {

		/*
		 * Update voxel volume mipmap levels
		*/

		_voxelVolume->SetVolumeMipmapLevels (settings.vct_mipmap_levels);
	}
}
