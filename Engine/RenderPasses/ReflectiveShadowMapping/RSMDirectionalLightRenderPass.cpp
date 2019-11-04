#include "RSMDirectionalLightRenderPass.h"

#include "Managers/ShaderManager.h"

#include "Renderer/Pipeline.h"

RSMDirectionalLightRenderPass::RSMDirectionalLightRenderPass () :
	_shadowShaderName ("REFLECTIVE_SHADOW_MAP_DIRECTIONAL_LIGHT")
{

}

void RSMDirectionalLightRenderPass::Init (const RenderSettings& settings)
{
	/*
	 * Shader for directional light with shadow casting
	*/

	ShaderManager::Instance ()->AddShader (_shadowShaderName,
		"Assets/Shaders/ReflectiveShadowMapping/reflectiveDeferredDirVolShadowMapLightVertex.glsl",
		"Assets/Shaders/ReflectiveShadowMapping/reflectiveDeferredDirVolShadowMapLightFragment.glsl");
}

void RSMDirectionalLightRenderPass::Clear ()
{

}

void RSMDirectionalLightRenderPass::LockShader (const RenderLightObject* renderLightObject)
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

std::vector<PipelineAttribute> RSMDirectionalLightRenderPass::GetCustomAttributes (const RenderSettings& settings) const
{
	std::vector<PipelineAttribute> attributes;

	PipelineAttribute rsmBias;

	rsmBias.type = PipelineAttribute::AttrType::ATTR_1F;

	rsmBias.name = "rsmBias";

	rsmBias.value.x = settings.rsm_bias;

	attributes.push_back (rsmBias);

	return attributes;
}
