#include "DeferredPointLightRenderPass.h"

#include "Resources/Resources.h"
#include "Renderer/RenderSystem.h"

#include "Renderer/Pipeline.h"

void DeferredPointLightRenderPass::Init (const RenderSettings& settings)
{
	/*
	 * Initialize volumetric light render pass
	*/

	VolumetricLightRenderPass::Init (settings);

	/*
	 * Shader for general directional light with no shadow casting
	*/

	Resource<Shader> shader = Resources::LoadShader ({
		"Assets/Shaders/deferredPointVolLightVertex.glsl",
		"Assets/Shaders/deferredPointVolLightFragment.glsl"
	});

	_shaderView = RenderSystem::LoadShader (shader);

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
		Pipeline::LockShader (_shaderView);
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
