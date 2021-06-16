#include "VolumetricLightingDirectionalRenderPass.h"

#include "Resources/Resources.h"
#include "Renderer/RenderSystem.h"

#include "Renderer/Pipeline.h"

#include "Debug/Statistics/StatisticsManager.h"
#include "VolLightingStatisticsObject.h"

void VolumetricLightingDirectionalRenderPass::Init (const RenderSettings& settings)
{
	/*
	 * Shader for general directional light with no shadow casting
	*/

	Resource<Shader> shader = Resources::LoadShader ({
		"Assets/Shaders/deferredDirVolLightVertex.glsl",
		"Assets/Shaders/VolumetricLighting/volumetricLightingDirectionalFragment.glsl"
	});

	_shadowShaderView = RenderSystem::LoadShader (shader);
}

RenderVolumeCollection* VolumetricLightingDirectionalRenderPass::Execute (const RenderScene* renderScene, const Camera* camera,
	const RenderSettings& settings, RenderVolumeCollection* rvc)
{
	/*
	 * Bind light accumulation volume
	*/

	auto resultVolume = (FramebufferRenderVolume*) rvc->GetRenderVolume ("volumetricLightMap");

	resultVolume->GetFramebufferView ()->Activate ();

	GL::ClearColor (0, 0, 0, 0);
	GL::Clear (GL_COLOR_BUFFER_BIT);

	/*
	 * Update statistics object
	*/

	auto volLightingStatisticsObject = StatisticsManager::Instance ()->GetStatisticsObject <VolLightingStatisticsObject> ();

	volLightingStatisticsObject->volumetricLightMapVolume = resultVolume;

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

void VolumetricLightingDirectionalRenderPass::Clear ()
{
	/*
	 * Nothing
	*/
}

void VolumetricLightingDirectionalRenderPass::LockShader (const RenderLightObject* renderLightObject)
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

std::vector<PipelineAttribute> VolumetricLightingDirectionalRenderPass::GetCustomAttributes (const RenderSettings& settings) const
{
	std::vector<PipelineAttribute> attributes;

	/*
	 * Attach volumetric lighting attributes to pipeline
	*/

	PipelineAttribute volLightingIterations;
	PipelineAttribute volLightingScattering;
	PipelineAttribute volLightingIntensity;

	volLightingIterations.type = PipelineAttribute::AttrType::ATTR_1I;
	volLightingScattering.type = PipelineAttribute::AttrType::ATTR_1F;
	volLightingIntensity.type = PipelineAttribute::AttrType::ATTR_1F;

	volLightingIterations.name = "volLightingIterations";
	volLightingScattering.name = "volLightingScattering";
	volLightingIntensity.name = "volLightingIntensity";

	volLightingIterations.value.x = settings.vol_lighting_iterations;
	volLightingScattering.value.x = settings.vol_lighting_scattering;
	volLightingIntensity.value.x = settings.vol_lighting_intensity;

	attributes.push_back (volLightingIterations);
	attributes.push_back (volLightingScattering);
	attributes.push_back (volLightingIntensity);

	return attributes;
}
