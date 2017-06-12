#include "VoxelizationRenderPass.h"

#include "Managers/ShaderManager.h"

#include "Renderer/Pipeline.h"

#include "Systems/Window/Window.h"

#include "Wrappers/OpenGL/GL.h"

#include "Core/Console/Console.h"

/*
 * TODO: Move this to settings
*/

#define VOLUME_DIMENSTIONS 512

VoxelizationRenderPass::VoxelizationRenderPass () :
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

	_voxelVolume->Init (VOLUME_DIMENSTIONS);

	/*
	* Voxelization shader init
	*/

	ShaderManager::Instance ()->AddShader ("VOXELIZATION_PASS_SHADER",
		"Assets/Shaders/Voxelize/voxelizeVertex.glsl",
		"Assets/Shaders/Voxelize/voxelizeFragment.glsl",
		"Assets/Shaders/Voxelize/voxelizeGeometry.glsl");
}

RenderVolumeCollection* VoxelizationRenderPass::Execute (Scene* scene, Camera* camera, RenderVolumeCollection* rvc)
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

	/*
	* Lock voxelization shader for geomtry rendering
	*/

	Pipeline::LockShader (ShaderManager::Instance ()->GetShader ("VOXELIZATION_PASS_SHADER"));
}

void VoxelizationRenderPass::GeometryVoxelizationPass (Scene* scene)
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
	 * Send voxel volume attributes to pipeline
	*/

	Pipeline::SendCustomAttributes ("VOXELIZATION_PASS_SHADER", 
		_voxelVolume->GetCustomAttributes ());

	/*
	* Render geometry
	*/

	for (SceneObject* sceneObject : *scene) {
		if (sceneObject->GetRenderer ()->GetStageType () != Renderer::StageType::DEFERRED_STAGE) {
			continue;
		}

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

void VoxelizationRenderPass::UpdateVoxelVolumeBoundingBox (Scene* scene)
{
	AABBVolume* boundingBox = scene->GetBoundingBox ();
	AABBVolume::AABBVolumeInformation* volume = boundingBox->GetVolumeInformation ();

	glm::vec3 minVertex = volume->minVertex;
	glm::vec3 maxVertex = volume->maxVertex;

	_voxelVolume->UpdateBoundingBox (minVertex, maxVertex);
}
