#include "VoxelizationRenderPass.h"

#include "Managers/ShaderManager.h"

#include "Renderer/Pipeline.h"

#include "Systems/Window/Window.h"

#include "Wrappers/OpenGL/GL.h"

#include "Core/Console/Console.h"

#include "SceneNodes/SceneLayer.h"

/*
 * TODO: Move this to settings
*/

#define VOLUME_DIMENSTIONS 512

VoxelizationRenderPass::VoxelizationRenderPass () :
	_staticShaderName ("VOXELIZATION_PASS_STATIC_SHADER"),
	_animationShaderName ("VOXELIZATION_PASS_ANIMATION_SHADER"),
	_voxelVolume (new VoxelVolume ())
{

}

VoxelizationRenderPass::~VoxelizationRenderPass ()
{
	delete _voxelVolume;
}

void VoxelizationRenderPass::Init ()
{
	/*
	 * Initialize voxel volume
	*/

	if (!_voxelVolume->Init (VOLUME_DIMENSTIONS)) {
		Console::LogError (std::string () +
			"Voxel texture cannot be initialized!" +
			" It is not possible to continue the process. End now!");
		exit (VOXEL_TEXTURE_NOT_INIT);
	}

	/*
	 * Shader for static objects
	*/

	ShaderManager::Instance ()->AddShader (_staticShaderName,
		"Assets/Shaders/Voxelize/voxelizeVertex.glsl",
		"Assets/Shaders/Voxelize/voxelizeFragment.glsl",
		"Assets/Shaders/Voxelize/voxelizeGeometry.glsl");

	/*
	 * Shader for animated objects
	*/

	ShaderManager::Instance ()->AddShader (_animationShaderName,
		"Assets/Shaders/Voxelize/voxelizeAnimationVertex.glsl",
		"Assets/Shaders/Voxelize/voxelizeFragment.glsl",
		"Assets/Shaders/Voxelize/voxelizeGeometry.glsl");
}

RenderVolumeCollection* VoxelizationRenderPass::Execute (const Scene* scene, const Camera* camera, RenderVolumeCollection* rvc)
{
	/*
	* Voxelization start
	*/

	StartVoxelization ();

	/*
	* Voxelization: voxelize geomtry
	*/

	GeometryVoxelizationPass (scene);

	/*
	* Clear opengl state after voxelization
	*/

	EndVoxelization ();

	/*
	 * Send back the collection with voxel volume attached
	*/

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
	GL::Viewport (0, 0, VOLUME_DIMENSTIONS, VOLUME_DIMENSTIONS);
}

void VoxelizationRenderPass::GeometryVoxelizationPass (const Scene* scene)
{
	/*
	* Update voxel volume based on scene bounding box
	*/

	UpdateVoxelVolumeBoundingBox (scene);

	/*
	* Bind voxel volume to geometry render pass
	*/

	_voxelVolume->BindForWriting ();

	/*
	* Render geometry
	*/

	for (SceneObject* sceneObject : *scene) {
		if (sceneObject->GetRenderer ()->GetStageType () != Renderer::StageType::DEFERRED_STAGE) {
			continue;
		}

		/*
		 * Lock voxelization shader for geomtry rendering
		*/

		LockShader (sceneObject->GetLayers ());

		/*
		 * Send voxel volume attributes to pipeline
		*/

		Pipeline::SendCustomAttributes ("", _voxelVolume->GetCustomAttributes ());

		/*
		 * Voxelize object
		*/

		sceneObject->GetRenderer ()->Draw ();
	}
}

void VoxelizationRenderPass::EndVoxelization ()
{
	// GL::MemoryBarrier(GL_ALL_BARRIER_BITS);

	/*
	* Clear settings
	*/

	GL::Viewport (0, 0, Window::GetWidth (), Window::GetHeight ());
	GL::ColorMask (GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
	GL::DepthMask (GL_TRUE);

	GL::Enable (GL_CULL_FACE);

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
		Pipeline::LockShader (ShaderManager::Instance ()->GetShader (_animationShaderName));
	}

	/*
	 * Lock general shader for not animated objects
	*/

	if (sceneLayers & (SceneLayer::STATIC | SceneLayer::DYNAMIC)) {
		Pipeline::LockShader (ShaderManager::Instance ()->GetShader (_staticShaderName));
	}
}

void VoxelizationRenderPass::UpdateVoxelVolumeBoundingBox (const Scene* scene)
{
	AABBVolume* boundingBox = scene->GetBoundingBox ();
	AABBVolume::AABBVolumeInformation* volume = boundingBox->GetVolumeInformation ();

	glm::vec3 minVertex = volume->minVertex;
	glm::vec3 maxVertex = volume->maxVertex;

	_voxelVolume->UpdateBoundingBox (minVertex, maxVertex);
}
