#include "RSMDirectionalLightContainerRenderSubPass.h"

#include "Managers/ShaderManager.h"

#include "Renderer/Pipeline.h"

RSMDirectionalLightContainerRenderSubPass::RSMDirectionalLightContainerRenderSubPass () :
	_shadowShaderName ("REFLECTIVE_SHADOW_MAP_DIRECTIONAL_LIGHT")
{

}

void RSMDirectionalLightContainerRenderSubPass::Init (const RenderSettings& settings)
{
	/*
	 * Shader for directional light with shadow casting
	*/

	ShaderManager::Instance ()->AddShader (_shadowShaderName,
		"Assets/Shaders/ReflectiveShadowMapping/reflectiveDeferredDirVolShadowMapLightVertex.glsl",
		"Assets/Shaders/ReflectiveShadowMapping/reflectiveDeferredDirVolShadowMapLightFragment.glsl");
}

void RSMDirectionalLightContainerRenderSubPass::Clear ()
{

}

void RSMDirectionalLightContainerRenderSubPass::LockShader (const RenderLightObject* renderLightObject)
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

std::vector<PipelineAttribute> RSMDirectionalLightContainerRenderSubPass::GetCustomAttributes (const RenderSettings& settings) const
{
	std::vector<PipelineAttribute> attributes;

	PipelineAttribute rsmBias;
	PipelineAttribute rsmRadius;
	PipelineAttribute rsmIntensity;

	rsmBias.type = PipelineAttribute::AttrType::ATTR_1F;
	rsmRadius.type = PipelineAttribute::AttrType::ATTR_1F;
	rsmIntensity.type = PipelineAttribute::AttrType::ATTR_1F;

	rsmBias.name = "rsmBias";
	rsmRadius.name = "rsmRadius";
	rsmIntensity.name = "rsmIntensity";

	rsmBias.value.x = settings.rsm_bias;
	rsmRadius.value.x = settings.rsm_radius;
	rsmIntensity.value.x = settings.rsm_intensity;

	attributes.push_back (rsmBias);
	attributes.push_back (rsmRadius);
	attributes.push_back (rsmIntensity);

	return attributes;
}
