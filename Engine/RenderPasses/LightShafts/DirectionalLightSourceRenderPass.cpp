#include "DirectionalLightSourceRenderPass.h"

#include "Resources/Resources.h"
#include "Renderer/RenderSystem.h"

#include "Renderer/Pipeline.h"

#include "Debug/Statistics/StatisticsManager.h"
#include "LightShaftsStatisticsObject.h"

void DirectionalLightSourceRenderPass::Init (const RenderSettings& settings)
{
	/*
	 * Shader for general directional light with no shadow casting
	*/

	Resource<Shader> shader = Resources::LoadShader ({
		"Assets/Shaders/deferredDirVolLightVertex.glsl",
		"Assets/Shaders/LightShafts/dirLightSourceFragment.glsl"
	});

	_shadowShaderView = RenderSystem::LoadShader (shader);
}

RenderVolumeCollection* DirectionalLightSourceRenderPass::Execute (const RenderScene* renderScene, const Camera* camera,
	const RenderSettings& settings, RenderVolumeCollection* rvc)
{
	/*
	 * Bind light accumulation volume
	*/

	auto resultVolume = (FramebufferRenderVolume*) rvc->GetRenderVolume ("lightSourceMap");

	resultVolume->GetFramebufferView ()->Activate ();

	GL::ClearColor (0, 0, 0, 0);
	GL::Clear (GL_COLOR_BUFFER_BIT);

	/*
	 * Update statistics object
	*/

	auto lightShaftsStatisticsObject = StatisticsManager::Instance ()->GetStatisticsObject <LightShaftsStatisticsObject> ();

	lightShaftsStatisticsObject->lightShaftsMapVolume = resultVolume;

	/*
	 * Draw volumetric lights
	*/

	DirectionalLightPass (renderScene, camera, settings, rvc);

	/*
	 * End directional light pass
	*/

	EndDirectionalLightPass ();

	return rvc;
}

void DirectionalLightSourceRenderPass::Clear ()
{
	/*
	 * Nothing
	*/
}

void DirectionalLightSourceRenderPass::LockShader (const RenderLightObject* renderLightObject)
{
	/*
	 * Unlock last shader
	*/

	Pipeline::UnlockShader ();

	/*
	 * Lock shader for shadow directional light
	*/

	if (renderLightObject->IsCastingShadows () == true) {
		Pipeline::LockShader (_shadowShaderView);
	}
}

std::vector<PipelineAttribute> DirectionalLightSourceRenderPass::GetCustomAttributes (const RenderSettings& settings) const
{
	std::vector<PipelineAttribute> attributes;

	return attributes;
}
