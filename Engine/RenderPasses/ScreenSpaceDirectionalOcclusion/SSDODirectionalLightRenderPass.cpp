#include "SSDODirectionalLightRenderPass.h"

#include "Managers/ShaderManager.h"

#include "Renderer/Pipeline.h"

SSDODirectionalLightRenderPass::SSDODirectionalLightRenderPass () :
	_shadowShaderName ("SCREEN_SPACE_DIRECTIONAL_OCCLUSION_SHADOW_MAP_DIRECTIONAL_LIGHT")
{

}

void SSDODirectionalLightRenderPass::Init (const RenderSettings& settings)
{
	/*
	 * Shader for general directional light with no shadow casting
	*/

	ShaderManager::Instance ()->AddShader (_shadowShaderName,
		"Assets/Shaders/ScreenSpaceDirectionalOcclusion/screenSpaceDirectionalOcclusionLightVertex.glsl",
		"Assets/Shaders/ScreenSpaceDirectionalOcclusion/screenSpaceDirectionalOcclusionLightFragment.glsl");
}

void SSDODirectionalLightRenderPass::Clear ()
{

}

void SSDODirectionalLightRenderPass::LockShader (const RenderLightObject* renderLightObject)
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

std::vector<PipelineAttribute> SSDODirectionalLightRenderPass::GetCustomAttributes (const RenderSettings& settings) const
{
	std::vector<PipelineAttribute> attributes;

	PipelineAttribute indirectIntensity;

	indirectIntensity.type = PipelineAttribute::AttrType::ATTR_1F;

	indirectIntensity.name = "indirectIntensity";

	indirectIntensity.value.x = settings.vct_indirect_intensity;

	attributes.push_back (indirectIntensity);

	return attributes;
}
