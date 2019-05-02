#include "DeferredAmbientLightContainerRenderSubPass.h"

#include "Systems/Settings/SettingsManager.h"

#include "Lighting/LightsManager.h"

#include "Managers/ShaderManager.h"

#include "Renderer/Pipeline.h"

DeferredAmbientLightContainerRenderSubPass::DeferredAmbientLightContainerRenderSubPass () :
	_shaderName ("AMBIENT_LIGHT"),
	_aoShaderName ("AMBIENT_OCCLUSION_AMBIENT_LIGHT")
{

}

DeferredAmbientLightContainerRenderSubPass::~DeferredAmbientLightContainerRenderSubPass ()
{

}

void DeferredAmbientLightContainerRenderSubPass::Init (const RenderSettings& settings)
{
	/*
	 * Shader for ambient light without ambient occlusion
	*/

	ShaderManager::Instance ()->AddShader (_shaderName,
		"Assets/Shaders/PostProcess/postProcessVertex.glsl",
		"Assets/Shaders/deferredAmbientLightFragment.glsl");

	/*
	 * Shader for ambient light with ambient occlusion
	*/

	ShaderManager::Instance ()->AddShader (_aoShaderName,
		"Assets/Shaders/PostProcess/postProcessVertex.glsl",
		"Assets/Shaders/deferredAmbientOcclusionAmbientLightFragment.glsl");
}

bool DeferredAmbientLightContainerRenderSubPass::IsAvailable (const Scene* scene, const Camera* camera,
	const RenderSettings& settings, const RenderVolumeCollection* rvc) const
{
	/*
	 * Always available
	*/

	return true;
}

RenderVolumeCollection* DeferredAmbientLightContainerRenderSubPass::Execute (const Scene* scene, const Camera* camera,
	const RenderSettings& settings, RenderVolumeCollection* rvc)
{
	/*
	 * Start ambient light pass
	*/

	StartAmbientLightPass (settings, rvc);

	/*
	 * Render ambient light
	*/

	AmbientLightPass (scene, camera, settings, rvc);

	/*
	 * End directional light pass
	*/

	EndAmbientLightPass ();

	return rvc;
}

void DeferredAmbientLightContainerRenderSubPass::Clear ()
{
	/*
	 * Do nothing
	*/
}

void DeferredAmbientLightContainerRenderSubPass::StartAmbientLightPass (const RenderSettings& settings, RenderVolumeCollection* rvc)
{
	/*
	 * Lock ambient light shader
	*/

	LockShader (settings);

	/*
	 * Bind light accumulation framebuffer for writing
	*/

	auto lightAccumulationVolume = rvc->GetRenderVolume ("LightAccumulationVolume");

	lightAccumulationVolume->BindForWriting ();
}

void DeferredAmbientLightContainerRenderSubPass::AmbientLightPass (const Scene* scene, const Camera* camera,
	const RenderSettings& settings, RenderVolumeCollection* rvc)
{
	/*
	 * Bind all render volumes
	*/

	for (RenderVolumeI* renderVolume : *rvc) {
		renderVolume->BindForReading ();
	}

	/*
	 * Send custom uniforms
	*/

	Pipeline::SendCustomAttributes ("", GetCustomAttributes (rvc));

	/*
	 * Set viewport
	*/

	GL::Viewport (settings.viewport.x, settings.viewport.y,
		settings.viewport.width, settings.viewport.height);

	/*
	 * Disable depth test
	*/

	GL::Disable (GL_DEPTH_TEST);

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
	 * Draw a screen covering triangle
	*/

	GL::DrawArrays (GL_TRIANGLES, 0, 3);
}

void DeferredAmbientLightContainerRenderSubPass::EndAmbientLightPass ()
{
	/*
	 * Unlock current locked shader for further rendering
	*/

	Pipeline::UnlockShader ();
}

void DeferredAmbientLightContainerRenderSubPass::LockShader (const RenderSettings& settings)
{
	/*
	 * Lock shader without ambient occlusion
	*/

	if (settings.ssao_enabled == false) {
		Pipeline::LockShader (ShaderManager::Instance ()->GetShader (_shaderName));
	}

	/*
	 * Lock shader with ambient occlusion
	*/

	if (settings.ssao_enabled == true) {
		Pipeline::LockShader (ShaderManager::Instance ()->GetShader (_aoShaderName));
	}
}

std::vector<PipelineAttribute> DeferredAmbientLightContainerRenderSubPass::GetCustomAttributes (RenderVolumeCollection* rvc)
{
	std::vector<PipelineAttribute> attributes;

	/*
	 * Attach all volume attributes to pipeline
	*/

	for (RenderVolumeI* renderVolume : *rvc) {
		std::vector<PipelineAttribute> volumeAttributes = renderVolume->GetCustomAttributes ();

		attributes.insert (attributes.end (), volumeAttributes.begin (), volumeAttributes.end ());
	}

	/*
	 * Add ambient light color
	*/

	PipelineAttribute ambientLightColor;

	ambientLightColor.type = PipelineAttribute::AttrType::ATTR_3F;

	ambientLightColor.name = "ambientLightColor";

	ambientLightColor.value = LightsManager::Instance ()->GetAmbientLightColor ().ToVector3 ();

	attributes.push_back (ambientLightColor);

	return attributes;
}
