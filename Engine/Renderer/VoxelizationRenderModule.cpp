#include "VoxelizationRenderModule.h"

#include <vector>
#include <algorithm>

#include "Renderer.h"

#include "Pipeline.h"

#include "Wrappers/OpenGL/GL.h"

#include "Systems/Window/Window.h"

#include "Debug/Profiler/Profiler.h"

#define VOLUME_DIMENSTIONS 512

VoxelizationRenderModule::VoxelizationRenderModule () :
	_voxelVolume (new VoxelVolume ())
{
	_voxelVolume->Init (VOLUME_DIMENSTIONS);
}

VoxelizationRenderModule::~VoxelizationRenderModule ()
{
	delete _voxelVolume;
}

void VoxelizationRenderModule::RenderScene (Scene* scene, Camera* camera)
{
	/*
	 * Send Camera to Pipeline
	*/

	UpdateCamera (camera);

	/*
	 * Voxelize the scene
	*/

	static bool firstTime = true;

	if (firstTime) {
		VoxelizePass(scene);
		firstTime = false;
	}

	/*
	 * Voxel Ray Trace Pass
	*/

	VoxelRayTracePass ();
}


void VoxelizationRenderModule::VoxelizePass (Scene* scene)
{
	PROFILER_LOGGER ("Voxelize Pass");

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
}

void VoxelizationRenderModule::StartVoxelization()
{
	_voxelVolume->ClearVoxels();

	_voxelVolume->StartVoxelizationPass ();
}

void VoxelizationRenderModule::GeometryVoxelizationPass(Scene* scene)
{
	/*
	 * Update voxel volume based on scene bounding box
	*/

	UpdateVoxelVolumeBoundingBox (scene);

	/*
	 * Bind voxel volume to geometry render pass
	*/

	_voxelVolume->BindForVoxelizationPass();

	/*
	 * Render geometry 
	*/
	
	for (SceneObject* sceneObject : *scene) {
		if (sceneObject->GetRenderer()->GetStageType() != Renderer::StageType::DEFERRED_STAGE) {
			continue;
		}

		sceneObject->GetRenderer()->Draw();
	}
}

void VoxelizationRenderModule::EndVoxelization()
{
	_voxelVolume->EndVoxelizationPass ();
}

void VoxelizationRenderModule::UpdateVoxelVolumeBoundingBox(Scene* scene)
{
	AABBVolume* boundingBox = scene->GetBoundingBox();
	AABBVolume::AABBVolumeInformation* volume = boundingBox->GetVolumeInformation();

	glm::vec3 minVertex = volume->minVertex;
	glm::vec3 maxVertex = volume->maxVertex;

	_voxelVolume->UpdateBoundingBox (minVertex, maxVertex);
}

void VoxelizationRenderModule::VoxelRayTracePass ()
{
	PROFILER_LOGGER ("Ray Trace Pass");

	/*
	 * Start Ray Trace Pass
	*/

	StartRayTrace ();

	/*
	 * Ray Trace Rendering Pass
	*/

	VoxelRenderingRayTracePass ();
}

void VoxelizationRenderModule::StartRayTrace()
{
	_voxelVolume->StartRayTracePass ();
}

void VoxelizationRenderModule::VoxelRenderingRayTracePass()
{
	_voxelVolume->BindForRayTracePass ();

	/*
	 * Clear framebuffer
	*/

	GL::Clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	/*
	 * Render fullscreen quad.	
	*/

	GL::EnableVertexAttribArray(0);

	GL::Begin(GL_QUADS);
		GL::VertexAttrib2f(0, -1.0f, -1.0f);
		GL::VertexAttrib2f(0, 1.0f, -1.0f);
		GL::VertexAttrib2f(0, 1.0f, 1.0f);
		GL::VertexAttrib2f(0, -1.0f, 1.0f);
	GL::End();
}

/*
 * TODO: Move this part somewhere else because it belongs to another
 * abstraction layer. This class only work with objects rendering, not
 * pipeline's job
*/
	
void VoxelizationRenderModule::UpdateCamera (Camera* camera)
{
	// Create Projection
	Pipeline::CreateProjection (camera);

	// Create View Matrix
	Pipeline::SendCamera (camera);	
}
