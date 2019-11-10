#include "DeferredPointLightRenderPass.h"

#include "Managers/ShaderManager.h"

#include "Renderer/Pipeline.h"

DeferredPointLightRenderPass::DeferredPointLightRenderPass () :
	_shaderName ("POINT_LIGHT"),
	_shadowShaderName ("SHADOW_MAP_POINT_LIGHT")
{

}

void DeferredPointLightRenderPass::Init (const RenderSettings& settings)
{
	/*
	 * Initialize volumetric light render pass
	*/

	VolumetricLightRenderPass::Init (settings);

	/*
	 * Shader for general directional light with no shadow casting
	*/

	ShaderManager::Instance ()->AddShader (_shaderName,
		"Assets/Shaders/deferredPointVolLightVertex.glsl",
		"Assets/Shaders/deferredPointVolLightFragment.glsl");

	/*
	 * Shader for directional light with shadow casting
	*/

	// ShaderManager::Instance ()->AddShader (_shadowShaderName,
	// 	"Assets/Shaders/ShadowMap/deferredDirVolShadowMapLightVertex.glsl",
	// 	"Assets/Shaders/ShadowMap/deferredDirVolShadowMapLightFragment.glsl");
}

void DeferredPointLightRenderPass::Clear ()
{
	/*
	 * Nothing
	*/
}

void DeferredPointLightRenderPass::LockShader (const RenderLightObject* renderLightObject)
{
	/*
	 * Unlock last shader
	*/

	Pipeline::UnlockShader ();

	/*
	 * Lock shader for shadow directional light
	*/

	// if (volumetricLight->IsCastingShadows () == true) {
	// 	Pipeline::LockShader (ShaderManager::Instance ()->GetShader (_shadowShaderName));
	// }

	/*
	 * Lock general shader for directional light
	*/

	if (renderLightObject->IsCastingShadows () == false) {
		Pipeline::LockShader (ShaderManager::Instance ()->GetShader (_shaderName));
	}
}

std::vector<PipelineAttribute> DeferredPointLightRenderPass::GetCustomAttributes (const RenderLightObject* renderLightObject) const
{
	auto renderPointLightObject = dynamic_cast<const RenderPointLightObject*> (renderLightObject);

	std::vector<PipelineAttribute> attributes;

	PipelineAttribute lightRange;

	lightRange.type = PipelineAttribute::AttrType::ATTR_1F;

	lightRange.name = "lightRange";

	lightRange.value.x = renderPointLightObject->GetLightRange ();

	attributes.push_back (lightRange);

	return attributes;
}
