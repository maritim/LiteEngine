#include "VCTDirectionalLightRenderPass.h"

#include "Resources/Resources.h"
#include "Renderer/RenderSystem.h"

#include "Renderer/Pipeline.h"

void VCTDirectionalLightRenderPass::Init (const RenderSettings& settings)
{
	/*
	 * Shader for general directional light with no shadow casting
	*/

	Resource<Shader> shadowShader = Resources::LoadShader ({
		"Assets/Shaders/VoxelConeTracing/voxelConeTracingVertex.glsl",
		"Assets/Shaders/VoxelConeTracing/voxelConeTracingFragment.glsl"
	});

	_shadowShaderView = RenderSystem::LoadShader (shadowShader);
}

void VCTDirectionalLightRenderPass::Clear ()
{

}

void VCTDirectionalLightRenderPass::LockShader (const RenderLightObject* renderLightObject)
{
	/*
	 * Unlock last shader
	*/

	Pipeline::UnlockShader ();

	/*
	 * Lock shader for shadow directional light
	*/

	Pipeline::LockShader (_shadowShaderView);
}

std::vector<PipelineAttribute> VCTDirectionalLightRenderPass::GetCustomAttributes (const RenderSettings& settings) const
{
	std::vector<PipelineAttribute> attributes;

	PipelineAttribute shadowConeRatio;
	PipelineAttribute shadowConeDistance;
	PipelineAttribute originBias;

	shadowConeRatio.type = PipelineAttribute::AttrType::ATTR_1F;
	shadowConeDistance.type = PipelineAttribute::AttrType::ATTR_1F;
	originBias.type = PipelineAttribute::AttrType::ATTR_1F;

	shadowConeRatio.name = "shadowConeRatio";
	shadowConeDistance.name = "shadowConeDistance";
	originBias.name = "originBias";

	shadowConeRatio.value.x = settings.vct_shadow_cone_ratio;
	shadowConeDistance.value.x = settings.vct_shadow_cone_distance;
	originBias.value.x = settings.vct_origin_bias;

	attributes.push_back (shadowConeRatio);
	attributes.push_back (shadowConeDistance);
	attributes.push_back (originBias);

	return attributes;
}
