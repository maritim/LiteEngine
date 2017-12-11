#include "VoxelRayTraceRenderPass.h"

#include "Managers/ShaderManager.h"

#include "Renderer/Pipeline.h"

#include "Wrappers/OpenGL/GL.h"

VoxelRayTraceRenderPass::~VoxelRayTraceRenderPass ()
{

}

void VoxelRayTraceRenderPass::Init ()
{
	/*
	* Load voxel ray trace shader
	*/

	ShaderManager::Instance ()->AddShader ("VOXEL_RAY_TRACE_PASS_SHADER",
		"Assets/Shaders/Voxelize/voxelRayTraceVertex.glsl",
		"Assets/Shaders/Voxelize/voxelRayTraceFragment.glsl",
		"Assets/Shaders/Voxelize/voxelRayTraceGeometry.glsl");
}

RenderVolumeCollection* VoxelRayTraceRenderPass::Execute (Scene* scene, Camera* camera, RenderVolumeCollection* rvc)
{
	/*
	* Start Ray Trace Pass
	*/

	StartRayTrace ();

	/*
	* Ray Trace Rendering Pass
	*/

	VoxelRenderingRayTracePass (camera, rvc);

	return rvc;
}

void VoxelRayTraceRenderPass::StartRayTrace ()
{
	/*
	* Use voxel ray trace shader
	*/

	Pipeline::SetShader (ShaderManager::Instance ()->GetShader ("VOXEL_RAY_TRACE_PASS_SHADER"));
}

void VoxelRayTraceRenderPass::VoxelRenderingRayTracePass (Camera* camera, RenderVolumeCollection* rvc)
{
	/*
	 * Bind voxel volume for reading
	*/

	rvc->GetRenderVolume ("VoxelVolume")->BindForReading ();

	/*
	 * Send voxel volume attributes to pipeline
	*/

	Pipeline::SendCustomAttributes ("VOXEL_RAY_TRACE_PASS_SHADER",
		rvc->GetRenderVolume ("VoxelVolume")->GetCustomAttributes ());

	/*
	 * Send camera to pipeline
	*/

	Pipeline::CreateProjection (camera->GetProjectionMatrix ());
	Pipeline::SendCamera (camera);
	Pipeline::ClearObjectTransform ();
	Pipeline::UpdateMatrices (ShaderManager::Instance ()->GetShader ("VOXEL_RAY_TRACE_PASS_SHADER"));

	/*
	 * Disable face culling
	*/

	GL::Disable (GL_CULL_FACE);

	/*
	* Clear framebuffer
	*/

	GL::Clear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	/*
	* Render fullscreen quad.
	*/

	GL::DrawArrays (GL_POINTS, 0, 1);
}

