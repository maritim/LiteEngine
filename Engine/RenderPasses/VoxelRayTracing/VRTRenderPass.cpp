#include "VRTRenderPass.h"

#include "Resources/Resources.h"
#include "Renderer/RenderSystem.h"

#include "Renderer/Pipeline.h"

#include "Wrappers/OpenGL/GL.h"

VRTRenderPass::~VRTRenderPass ()
{

}

void VRTRenderPass::Init (const RenderSettings& settings)
{
	/*
	 * Load voxel ray trace shader
	*/

	Resource<Shader> shader = Resources::LoadShader ({
		"Assets/Shaders/VoxelRayTracing/voxelRayTracingVertex.glsl",
		"Assets/Shaders/VoxelRayTracing/voxelRayTracingFragment.glsl",
		"Assets/Shaders/VoxelRayTracing/voxelRayTracingGeometry.glsl"
	});

	_shaderView = RenderSystem::LoadShader (shader);
}

RenderVolumeCollection* VRTRenderPass::Execute (const RenderScene* renderScene, const Camera* camera,
	const RenderSettings& settings, RenderVolumeCollection* rvc)
{
	if (settings.vct_debug_show_voxels == true) {

		/*
		* Start Ray Trace Pass
		*/

		StartRayTrace (rvc);

		/*
		* Ray Trace Rendering Pass
		*/

		VoxelRenderingRayTracePass (camera, settings, rvc);		
	}

	return rvc;
}

void VRTRenderPass::Clear ()
{
	/*
	 * Nothing
	*/
}

void VRTRenderPass::StartRayTrace (RenderVolumeCollection* rvc)
{
	/*
	 * Bind light accumulation framebuffer for writing
	*/

	auto resultFrameBuffer = rvc->GetRenderVolume ("ResultFrameBuffer2DVolume");

	resultFrameBuffer->BindForWriting ();

	/*
	* Use voxel ray trace shader
	*/

	Pipeline::SetShader (_shaderView);
}

void VRTRenderPass::VoxelRenderingRayTracePass (const Camera* camera, const RenderSettings& settings, RenderVolumeCollection* rvc)
{
	/*
	 * Set viewport
	*/

	GL::Viewport (settings.viewport.x, settings.viewport.y,
		settings.viewport.width, settings.viewport.height);

	/*
	 * Bind voxel volume for reading
	*/

	rvc->GetRenderVolume ("VoxelVolume")->BindForReading ();

	/*
	 * Send voxel volume attributes to pipeline
	*/

	Pipeline::SendCustomAttributes (_shaderView,
		rvc->GetRenderVolume ("VoxelVolume")->GetCustomAttributes ());

	/*
	 * Send voxel ray tracing attributes to pipeline
	*/

	Pipeline::SendCustomAttributes (_shaderView, GetCustomAttributes (settings));

	/*
	 * Send camera to pipeline
	*/

	Pipeline::CreateProjection (camera->GetProjectionMatrix ());
	Pipeline::SendCamera (camera);
	Pipeline::ClearObjectTransform ();
	Pipeline::UpdateMatrices (_shaderView);

	/*
	 * Blend between point lights with same weight.
	*/

	GL::Enable (GL_BLEND);
	GL::BlendEquation (GL_FUNC_ADD);
	GL::BlendFunc (GL_ONE, GL_ONE);

	/*
	 * Disable face culling
	*/

	GL::Disable (GL_CULL_FACE);

	/*
	 * Disable depth test
	*/

	GL::Disable (GL_DEPTH_TEST);

	/*
	* Render fullscreen quad.
	*/

	GL::DrawArrays (GL_POINTS, 0, 1);
}

std::vector<PipelineAttribute> VRTRenderPass::GetCustomAttributes (const RenderSettings& settings) const
{
	std::vector<PipelineAttribute> attributes;

	PipelineAttribute volumeMipmapLevel;

	volumeMipmapLevel.type = PipelineAttribute::AttrType::ATTR_1I;

	volumeMipmapLevel.name = "volumeMipmapLevel";

	volumeMipmapLevel.value.x = settings.vct_debug_volume_mipmap_level;

	attributes.push_back (volumeMipmapLevel);

	return attributes;
}
