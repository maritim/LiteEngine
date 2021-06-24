#include "LightShaftsRenderPass.h"

#include "Resources/Resources.h"
#include "Renderer/RenderSystem.h"

#include "Renderer/Pipeline.h"

#include "RenderPasses/FramebufferRenderVolume.h"

#include "Debug/Statistics/StatisticsManager.h"
#include "LightShaftsStatisticsObject.h"

void LightShaftsRenderPass::Init (const RenderSettings& settings)
{
	/*
	 * Shader for general directional light with no shadow casting
	*/

	Resource<Shader> shader = Resources::LoadShader ({
		"Assets/Shaders/deferredDirVolLightVertex.glsl",
		"Assets/Shaders/LightShafts/lightShaftsFragment.glsl"
	});

	_shadowShaderView = RenderSystem::LoadShader (shader);
}

RenderVolumeCollection* LightShaftsRenderPass::Execute (const RenderScene* renderScene, const Camera* camera,
	const RenderSettings& settings, RenderVolumeCollection* rvc)
{
	/*
	 * Bind light accumulation volume
	*/

	auto resultVolume = (FramebufferRenderVolume*) rvc->GetRenderVolume ("lightShaftsMap");

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

void LightShaftsRenderPass::Clear ()
{
	/*
	 * Nothing
	*/
}

void LightShaftsRenderPass::LockShader (const RenderLightObject* renderLightObject)
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

std::vector<PipelineAttribute> LightShaftsRenderPass::GetCustomAttributes (const RenderSettings& settings) const
{
	std::vector<PipelineAttribute> attributes;

	/*
	 * Attach bloom attributes to pipeline
	*/

	PipelineAttribute lightShaftsIterations;
	PipelineAttribute lightShaftsDensity;
	PipelineAttribute lightShaftsWeight;
	PipelineAttribute lightShaftsDecay;
	PipelineAttribute lightShaftsIntensity;

	lightShaftsIterations.type = PipelineAttribute::AttrType::ATTR_1I;
	lightShaftsDensity.type = PipelineAttribute::AttrType::ATTR_1F;
	lightShaftsWeight.type = PipelineAttribute::AttrType::ATTR_1F;
	lightShaftsDecay.type = PipelineAttribute::AttrType::ATTR_1F;
	lightShaftsIntensity.type = PipelineAttribute::AttrType::ATTR_1F;

	lightShaftsIterations.name = "lightShaftsIterations";
	lightShaftsDensity.name = "lightShaftsDensity";
	lightShaftsWeight.name = "lightShaftsWeight";
	lightShaftsDecay.name = "lightShaftsDecay";
	lightShaftsIntensity.name = "lightShaftsIntensity";

	lightShaftsIterations.value.x = settings.light_shafts_iterations;
	lightShaftsDensity.value.x = settings.light_shafts_density;
	lightShaftsWeight.value.x = settings.light_shafts_weight;
	lightShaftsDecay.value.x = settings.light_shafts_decay;
	lightShaftsIntensity.value.x = settings.light_shafts_intensity;

	attributes.push_back (lightShaftsIterations);
	attributes.push_back (lightShaftsDensity);
	attributes.push_back (lightShaftsWeight);
	attributes.push_back (lightShaftsDecay);
	attributes.push_back (lightShaftsIntensity);

	return attributes;
}
