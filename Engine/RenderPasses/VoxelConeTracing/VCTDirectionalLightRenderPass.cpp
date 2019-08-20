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

	PipelineAttribute ambientLightColor;
	PipelineAttribute indirectIntensity;

	ambientLightColor.type = PipelineAttribute::AttrType::ATTR_3F;
	indirectIntensity.type = PipelineAttribute::AttrType::ATTR_1F;

	ambientLightColor.name = "ambientLightColor";
	indirectIntensity.name = "indirectIntensity";

	// ambientLightColor.value = LightsManager::Instance ()->GetAmbientLightColor ().ToVector3 ();
	indirectIntensity.value.x = settings.vct_indirect_intensity;

	attributes.push_back (ambientLightColor);
	attributes.push_back (indirectIntensity);

	return attributes;
}
