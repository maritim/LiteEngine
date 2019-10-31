#include "VCTDirectionalLightRenderPass.h"

#include "Managers/ShaderManager.h"

#include "Renderer/Pipeline.h"

VCTDirectionalLightRenderPass::VCTDirectionalLightRenderPass () :
	_shadowShaderName ("VOXEL_CONE_TRACE_SHADOW_MAP_DIRECTIONAL_LIGHT")
{

}

void VCTDirectionalLightRenderPass::Init (const RenderSettings& settings)
{
	/*
	 * Shader for general directional light with no shadow casting
	*/

	ShaderManager::Instance ()->AddShader (_shadowShaderName,
		"Assets/Shaders/VoxelConeTracing/voxelConeTracingVertex.glsl",
		"Assets/Shaders/VoxelConeTracing/voxelConeTracingFragment.glsl");
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

	Pipeline::LockShader (ShaderManager::Instance ()->GetShader (_shadowShaderName));
}

std::vector<PipelineAttribute> VCTDirectionalLightRenderPass::GetCustomAttributes (const RenderSettings& settings) const
{
	std::vector<PipelineAttribute> attributes;

	PipelineAttribute indirectIntensity;
	PipelineAttribute refractiveIndirectIntensity;
	PipelineAttribute diffuseConeDistance;
	PipelineAttribute specularConeDistance;
	PipelineAttribute refractiveConeRatio;
	PipelineAttribute refractiveConeDistance;
	PipelineAttribute shadowConeRatio;
	PipelineAttribute shadowConeDistance;

	indirectIntensity.type = PipelineAttribute::AttrType::ATTR_1F;
	refractiveIndirectIntensity.type = PipelineAttribute::AttrType::ATTR_1F;
	diffuseConeDistance.type = PipelineAttribute::AttrType::ATTR_1F;
	specularConeDistance.type = PipelineAttribute::AttrType::ATTR_1F;
	refractiveConeRatio.type = PipelineAttribute::AttrType::ATTR_1F;
	refractiveConeDistance.type = PipelineAttribute::AttrType::ATTR_1F;
	shadowConeRatio.type = PipelineAttribute::AttrType::ATTR_1F;
	shadowConeDistance.type = PipelineAttribute::AttrType::ATTR_1F;

	indirectIntensity.name = "indirectIntensity";
	refractiveIndirectIntensity.name = "refractiveIndirectIntensity";
	diffuseConeDistance.name = "diffuseConeDistance";
	specularConeDistance.name = "specularConeDistance";
	refractiveConeRatio.name = "refractiveConeRatio";
	refractiveConeDistance.name = "refractiveConeDistance";
	shadowConeRatio.name = "shadowConeRatio";
	shadowConeDistance.name = "shadowConeDistance";

	indirectIntensity.value.x = settings.vct_indirect_intensity;
	refractiveIndirectIntensity.value.x = settings.vct_indirect_refractive_intensity;
	diffuseConeDistance.value.x = settings.vct_diffuse_cone_distance;
	specularConeDistance.value.x = settings.vct_specular_cone_distance;
	refractiveConeRatio.value.x = settings.vct_refractive_cone_ratio;
	refractiveConeDistance.value.x = settings.vct_refractive_cone_distance;
	shadowConeRatio.value.x = settings.vct_shadow_cone_ratio;
	shadowConeDistance.value.x = settings.vct_shadow_cone_distance;

	attributes.push_back (indirectIntensity);
	attributes.push_back (refractiveIndirectIntensity);
	attributes.push_back (diffuseConeDistance);
	attributes.push_back (specularConeDistance);
	attributes.push_back (refractiveConeRatio);
	attributes.push_back (refractiveConeDistance);
	attributes.push_back (shadowConeRatio);
	attributes.push_back (shadowConeDistance);

	return attributes;
}
