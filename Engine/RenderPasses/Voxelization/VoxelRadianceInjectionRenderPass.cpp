#include "VoxelRadianceInjectionRenderPass.h"

#include "Resources/Resources.h"
#include "Renderer/RenderSystem.h"

#include "Renderer/Pipeline.h"

void VoxelRadianceInjectionRenderPass::Init (const RenderSettings& settings)
{
	/*
	 * Initialize post processing shader
	*/

	Resource<Shader> shader = Resources::LoadShader ({
		"Assets/Shaders/PostProcess/postProcessVertex.glsl",
		"Assets/Shaders/Voxelize/voxelRadianceInjectionFragment.glsl"
	});

	_shaderView = RenderSystem::LoadShader (shader);
}

void VoxelRadianceInjectionRenderPass::Clear ()
{

}

RenderVolumeCollection* VoxelRadianceInjectionRenderPass::Execute (const RenderScene* renderScene, const Camera* camera,
	const RenderSettings& settings, RenderVolumeCollection* rvc)
{
	/*
	 * Get volumetric light from render volume collection
	*/

	RenderLightObject* renderLightObject = GetRenderLightObject (rvc);

	/*
	 * Start screen space ambient occlusion generation pass
	*/

	StartPostProcessPass (rvc);

	/*
	 * Screen space ambient occlusion generation pass
	*/

	PostProcessPass (renderScene, camera, settings, renderLightObject, rvc);

	/*
	 * End screen space ambient occlusion generation pass
	*/

	EndPostProcessPass ();

	return rvc;
}

bool VoxelRadianceInjectionRenderPass::IsAvailable (const RenderLightObject*) const
{
	/*
	 * Always execute light propagation volumes radiance injection render pass
	*/

	return true;
}

void VoxelRadianceInjectionRenderPass::StartPostProcessPass (RenderVolumeCollection* rvc)
{
	/*
	 * Bind screen space ambient occlusion volume for writing
	*/

	((VoxelVolume*) rvc->GetRenderVolume ("VoxelVolume"))->BindForWriting ();
}

void VoxelRadianceInjectionRenderPass::PostProcessPass (const RenderScene* renderScene,
	const Camera* camera, const RenderSettings& settings,
	const RenderLightObject* renderLightObject, RenderVolumeCollection* rvc)
{
	/*
	 * Set viewport
	*/

	RenderLightObject::Shadow shadow = renderLightObject->GetShadow ();
	glm::ivec2 rsmSize = shadow.resolution;

	GL::Viewport (0, 0, rsmSize.x, rsmSize.y);

	/*
	 * No rendering target
	*/

	// GL::DrawBuffer (GL_NONE);

	/*
	 * Don't need to write the light on depth buffer.
	*/

	GL::Disable (GL_DEPTH_TEST);

	/*
	 * Disable face culling
	*/

	GL::Disable (GL_CULL_FACE);

	/*
	 * Lock post-process shader
	*/

	Pipeline::LockShader (_shaderView);

	/*
	 * Send custom uniforms
	*/

	Pipeline::SendCustomAttributes (nullptr, rvc->GetRenderVolume ("ReflectiveShadowMapVolume")->GetCustomAttributes ());
	Pipeline::SendCustomAttributes (nullptr, rvc->GetRenderVolume ("VoxelVolume")->GetCustomAttributes ());
	Pipeline::SendCustomAttributes (nullptr, GetCustomAttributes (settings, renderLightObject));

	/*
	 * Draw a screen covering triangle
	*/

	GL::DrawArrays (GL_TRIANGLES, 0, 3);
}

void VoxelRadianceInjectionRenderPass::EndPostProcessPass ()
{
	/*
	 * Unlock current shader
	*/

	Pipeline::UnlockShader ();
}

std::vector<PipelineAttribute> VoxelRadianceInjectionRenderPass::GetCustomAttributes (const RenderSettings& settings,
	const RenderLightObject* renderLightObject) const
{
	std::vector<PipelineAttribute> attributes;

	PipelineAttribute rsmResolution;

	rsmResolution.type = PipelineAttribute::AttrType::ATTR_2F;

	rsmResolution.name = "rsmResolution";

	RenderLightObject::Shadow shadow = renderLightObject->GetShadow ();

	rsmResolution.value = glm::vec3 (shadow.resolution, 0.0f);

	attributes.push_back (rsmResolution);

	return attributes;
}
