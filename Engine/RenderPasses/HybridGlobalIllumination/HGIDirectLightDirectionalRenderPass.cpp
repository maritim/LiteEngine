#include "HGIDirectLightDirectionalRenderPass.h"

#include "Resources/Resources.h"
#include "Renderer/RenderSystem.h"

#include "Renderer/Pipeline.h"

#include "Debug/Statistics/StatisticsManager.h"
#include "HGIStatisticsObject.h"

void HGIDirectLightDirectionalRenderPass::Init (const RenderSettings& settings)
{
	/*
	 * Shader for general directional light with no shadow casting
	*/

	Resource<Shader> shader = Resources::LoadShader ({
		"Assets/Shaders/deferredDirVolLightVertex.glsl",
		"Assets/Shaders/HybridGlobalIllumination/hybridGlobalIlluminationDirectLightDirectionalFragment.glsl"
	});

	_shadowShaderView = RenderSystem::LoadShader (shader);
}

RenderVolumeCollection* HGIDirectLightDirectionalRenderPass::Execute (const RenderScene* renderScene, const Camera* camera,
	const RenderSettings& settings, RenderVolumeCollection* rvc)
{
	/*
	 * Bind light accumulation volume
	*/

	auto resultVolume = (FramebufferRenderVolume*) rvc->GetRenderVolume ("directLightMap");

	resultVolume->GetFramebufferView ()->Activate ();

	GL::ClearColor (0, 0, 0, 0);
	GL::Clear (GL_COLOR_BUFFER_BIT);

	/*
	 * Update statistics object
	*/

	auto hgiStatisticsObject = StatisticsManager::Instance ()->GetStatisticsObject <HGIStatisticsObject> ();

	hgiStatisticsObject->hgiDirectMapVolume = resultVolume;

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

void HGIDirectLightDirectionalRenderPass::Clear ()
{
	/*
	 * Nothingf
	*/
}

void HGIDirectLightDirectionalRenderPass::LockShader (const RenderLightObject* renderLightObject)
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

std::vector<PipelineAttribute> HGIDirectLightDirectionalRenderPass::GetCustomAttributes (const RenderSettings& settings) const
{
	std::vector<PipelineAttribute> attributes;

	return attributes;
}
