#include "ReflectiveShadowMapRenderPass.h"

#include "Managers/ShaderManager.h"

#include "Renderer/Pipeline.h"

#include "Wrappers/OpenGL/GL.h"

ReflectiveShadowMapRenderPass::~ReflectiveShadowMapRenderPass ()
{

}

void ReflectiveShadowMapRenderPass::Init ()
{
	/*
	 * Load reflective shadow map test pass shader
	*/

	ShaderManager::Instance ()->AddShader ("RELFECTIVE_SHADOW_MAP_TEST_PASS_SHADER",
		"Assets/Shaders/ReflectiveShadowMap/reflectiveShadowMapTestVertex.glsl",
		"Assets/Shaders/ReflectiveShadowMap/reflectiveShadowMapTestFragment.glsl",
		"Assets/Shaders/ReflectiveShadowMap/reflectiveShadowMapTestGeometry.glsl");
}

RenderVolumeCollection* ReflectiveShadowMapRenderPass::Execute (Scene* scene, Camera* camera, RenderVolumeCollection* rvc)
{
	/*
	 * Start Frame
	*/

	StartFrame ();

	/*
	 * Reflective Shadow Map Test Pass
	*/

	ReflectiveShadowMapTestPass (camera, rvc);

	return rvc;
}

void ReflectiveShadowMapRenderPass::StartFrame ()
{
	/*
	 * Disable blending
	*/

	GL::Disable (GL_BLEND);

	/*
	 * Use reflective shadow map test pass shader
	*/

	Pipeline::SetShader (ShaderManager::Instance ()->GetShader ("RELFECTIVE_SHADOW_MAP_TEST_PASS_SHADER"));
}

void ReflectiveShadowMapRenderPass::ReflectiveShadowMapTestPass (Camera* camera, RenderVolumeCollection* rvc)
{
	/*
	 * Bind voxel volume for reading
	*/

	rvc->GetRenderVolume ("ReflectiveShadowMapVolume")->BindForReading ();

	/*
	 * Send voxel volume attributes to pipeline
	*/

	Pipeline::SendCustomAttributes ("RELFECTIVE_SHADOW_MAP_TEST_PASS_SHADER",
		rvc->GetRenderVolume ("ReflectiveShadowMapVolume")->GetCustomAttributes ());

	/*
	 * Clear framebuffer
	*/

	GL::Clear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	/*
	 * Render fullscreen quad.
	*/

	GL::DrawArrays (GL_POINTS, 0, 1);
}

