#include "LPVRadianceInjectionRenderPass.h"

#include "Resources/Resources.h"
#include "Renderer/RenderSystem.h"

#include "LPVVolume.h"
#include "LPVGeometryVolume.h"

#include "Renderer/Pipeline.h"

void LPVRadianceInjectionRenderPass::Init (const RenderSettings& settings)
{
	/*
	 * Initialize post processing shader
	*/

	Resource<Shader> shader = Resources::LoadShader ({
		"Assets/Shaders/PostProcess/postProcessVertex.glsl",
		"Assets/Shaders/LightPropagationVolumes/lightPropagationVolumesRadianceInjectionFragment.glsl"
	});

	_shaderView = RenderSystem::LoadShader (shader);
}

void LPVRadianceInjectionRenderPass::Clear ()
{
	/*
	 * Nothing
	*/
}

RenderVolumeCollection* LPVRadianceInjectionRenderPass::Execute (const RenderScene* renderScene, const Camera* camera,
	const RenderSettings& settings, RenderVolumeCollection* rvc)
{
	/*
	 * Get volumetric light from render volume collection
	*/

	RenderLightObject* renderLightObject = GetRenderLightObject (rvc);

	/*
	 * Start screen space ambient occlusion generation pass
	*/

	StartPostProcessPass ();

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

bool LPVRadianceInjectionRenderPass::IsAvailable (const RenderLightObject*) const
{
	/*
	 * Always execute light propagation volumes radiance injection render pass
	*/

	return true;
}

void LPVRadianceInjectionRenderPass::StartPostProcessPass ()
{
	/*
	 * Bind screen space ambient occlusion volume for writing
	*/
}

void LPVRadianceInjectionRenderPass::PostProcessPass (const RenderScene* renderScene,
	const Camera* camera, const RenderSettings& settings,
	const RenderLightObject* renderLightObject, RenderVolumeCollection* rvc)
{
	auto lpvVolume = (LPVVolume*) rvc->GetRenderVolume ("LightPropagationVolume");
	auto lpvGeometryVolume = (LPVGeometryVolume*) rvc->GetRenderVolume ("LPVGeometryVolume");

	lpvVolume->BindForWriting ();
	lpvGeometryVolume->BindForWriting ();

	/*
	 * Set viewport
	*/

	RenderLightObject::Shadow shadow = renderLightObject->GetShadow ();
	glm::ivec2 rsmSize = shadow.resolution / 4;

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
	Pipeline::SendCustomAttributes (nullptr, lpvVolume->GetCustomAttributes ());
	Pipeline::SendCustomAttributes (nullptr, GetCustomAttributes (settings, renderLightObject));

	/*
	 * Draw a screen covering triangle
	*/

	GL::DrawArrays (GL_TRIANGLES, 0, 3);
}

void LPVRadianceInjectionRenderPass::EndPostProcessPass ()
{
	/*
	 * Unlock current shader
	*/

	Pipeline::UnlockShader ();
}

std::vector<PipelineAttribute> LPVRadianceInjectionRenderPass::GetCustomAttributes (const RenderSettings& settings,
	const RenderLightObject* renderLightObject) const
{
	std::vector<PipelineAttribute> attributes;

	PipelineAttribute rsmResolution;
	PipelineAttribute lightDirection;
	PipelineAttribute injectionBias;

	rsmResolution.type = PipelineAttribute::AttrType::ATTR_2F;
	lightDirection.type = PipelineAttribute::AttrType::ATTR_3F;
	injectionBias.type = PipelineAttribute::AttrType::ATTR_1F;

	rsmResolution.name = "rsmResolution";
	lightDirection.name = "lightDirection";
	injectionBias.name = "injectionBias";

	RenderLightObject::Shadow shadow = renderLightObject->GetShadow ();

	glm::vec3 lightDir = renderLightObject->GetTransform ()->GetRotation () * glm::vec3 (0, 0, -1);
	lightDir = glm::normalize (lightDir);

	rsmResolution.value = glm::vec3 (shadow.resolution, 0.0f);
	lightDirection.value = lightDir;
	injectionBias.value.x = settings.lpv_injection_bias;

	attributes.push_back (rsmResolution);
	attributes.push_back (lightDirection);
	attributes.push_back (injectionBias);

	return attributes;
}
