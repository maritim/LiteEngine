#include "LPVGeometryInjectionRenderPass.h"

#include "Resources/Resources.h"
#include "Renderer/RenderSystem.h"

#include "Renderer/Pipeline.h"

#include "Core/Console/Console.h"

void LPVGeometryInjectionRenderPass::Init (const RenderSettings& settings)
{
	/*
	 * Initialize post processing shader
	*/

	Resource<Shader> shader = Resources::LoadShader ({
		"Assets/Shaders/PostProcess/postProcessVertex.glsl",
		"Assets/Shaders/LightPropagationVolumes/lightPropagationVolumesGeometryInjectionFragment.glsl"
	});

	_shaderView = RenderSystem::LoadShader (shader);
}

void LPVGeometryInjectionRenderPass::Clear ()
{

}

RenderVolumeCollection* LPVGeometryInjectionRenderPass::Execute (const RenderScene* renderScene, const Camera* camera,
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

bool LPVGeometryInjectionRenderPass::IsAvailable (const RenderLightObject*) const
{
	/*
	 * Always execute light propagation volumes radiance injection render pass
	*/

	return true;
}

void LPVGeometryInjectionRenderPass::StartPostProcessPass (RenderVolumeCollection* rvc)
{
	/*
	 * Bind screen space ambient occlusion volume for writing
	*/

	rvc->GetRenderVolume ("LPVGeometryVolume")->BindForWriting ();
}

void LPVGeometryInjectionRenderPass::PostProcessPass (const RenderScene* renderScene,
	const Camera* camera, const RenderSettings& settings,
	const RenderLightObject* renderLightObject, RenderVolumeCollection* rvc)
{
	/*
	 * Set viewport
	*/

	glm::ivec2 rsmSize = glm::ivec2 (settings.framebuffer.width, settings.framebuffer.height) / 4;

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

	Pipeline::SendCamera (camera);
	Pipeline::UpdateMatrices (nullptr);
	Pipeline::SendCustomAttributes (nullptr, rvc->GetRenderVolume ("GBuffer")->GetCustomAttributes ());
	Pipeline::SendCustomAttributes (nullptr, rvc->GetRenderVolume ("LPVGeometryVolume")->GetCustomAttributes ());
	Pipeline::SendCustomAttributes (nullptr, GetCustomAttributes (settings, renderLightObject));

	/*
	 * Draw a screen covering triangle
	*/

	GL::DrawArrays (GL_TRIANGLES, 0, 3);
}

void LPVGeometryInjectionRenderPass::EndPostProcessPass ()
{
	/*
	 * Unlock current shader
	*/

	Pipeline::UnlockShader ();
}

std::vector<PipelineAttribute> LPVGeometryInjectionRenderPass::GetCustomAttributes (const RenderSettings& settings,
	const RenderLightObject* renderLightObject) const
{
	std::vector<PipelineAttribute> attributes;

	PipelineAttribute lightDirection;

	lightDirection.type = PipelineAttribute::AttrType::ATTR_3F;

	lightDirection.name = "lightDirection";

	glm::vec3 lightDir = renderLightObject->GetTransform ()->GetRotation () * glm::vec3 (0, 0, -1);
	lightDir = glm::normalize (lightDir);

	lightDirection.value = lightDir;

	attributes.push_back (lightDirection);

	return attributes;
}
