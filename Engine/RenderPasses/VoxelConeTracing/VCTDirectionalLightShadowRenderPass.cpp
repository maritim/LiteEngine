#include "VCTDirectionalLightShadowRenderPass.h"

#include "Resources/Resources.h"
#include "Renderer/RenderSystem.h"

#include "Renderer/Pipeline.h"

#include "Debug/Statistics/StatisticsManager.h"
#include "VCTStatisticsObject.h"

void VCTDirectionalLightShadowRenderPass::Init (const RenderSettings& settings)
{
	/*
	 * Shader for general directional light with no shadow casting
	*/

	Resource<Shader> shader = Resources::LoadShader ({
		"Assets/Shaders/deferredDirVolLightVertex.glsl",
		"Assets/Shaders/VoxelConeTracing/voxelConeTracingShadowFragment.glsl"
	});

	_shadowShaderView = RenderSystem::LoadShader (shader);
}

RenderVolumeCollection* VCTDirectionalLightShadowRenderPass::Execute (const RenderScene* renderScene, const Camera* camera,
	const RenderSettings& settings, RenderVolumeCollection* rvc)
{
	/*
	 * Bind light accumulation volume
	*/

	auto resultVolume = (FramebufferRenderVolume*) rvc->GetRenderVolume ("vctShadowMap");

	resultVolume->GetFramebufferView ()->Activate ();

	GL::ClearColor (0, 0, 0, 0);
	GL::Clear (GL_COLOR_BUFFER_BIT);

	/*
	 * Update statistics object
	*/

	auto vctStatisticsObject = StatisticsManager::Instance ()->GetStatisticsObject <VCTStatisticsObject> ();

	vctStatisticsObject->vctShadowVolume = resultVolume;

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

void VCTDirectionalLightShadowRenderPass::Clear ()
{
	/*
	 * Nothing
	*/
}

bool VCTDirectionalLightShadowRenderPass::IsAvailable (const RenderLightObject* renderLightObject) const
{
	/*
	 * Execute shadow map sub pass only if light is casting shadows
	*/

	return renderLightObject->IsCastingShadows ();
}

void VCTDirectionalLightShadowRenderPass::LockShader (const RenderLightObject* renderLightObject)
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

std::vector<PipelineAttribute> VCTDirectionalLightShadowRenderPass::GetCustomAttributes (const RenderSettings& settings) const
{
	std::vector<PipelineAttribute> attributes;

	PipelineAttribute shadowConeEnabled;
	PipelineAttribute shadowConeRatio;
	PipelineAttribute shadowConeDistance;

	shadowConeEnabled.type = PipelineAttribute::AttrType::ATTR_1I;
	shadowConeRatio.type = PipelineAttribute::AttrType::ATTR_1F;
	shadowConeDistance.type = PipelineAttribute::AttrType::ATTR_1F;

	shadowConeEnabled.name = "shadowConeEnabled";
	shadowConeRatio.name = "shadowConeRatio";
	shadowConeDistance.name = "shadowConeDistance";

	shadowConeEnabled.value.x = settings.vct_shadow_cone_enabled;
	shadowConeRatio.value.x = settings.vct_shadow_cone_ratio;
	shadowConeDistance.value.x = settings.vct_shadow_cone_distance;

	attributes.push_back (shadowConeEnabled);
	attributes.push_back (shadowConeRatio);
	attributes.push_back (shadowConeDistance);

	return attributes;
}
