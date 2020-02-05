#include "DeferredSpotLightRenderPass.h"

#include "Resources/Resources.h"
#include "Renderer/RenderSystem.h"

#include "Renderer/Pipeline.h"

#include "Utils/Extensions/MathExtend.h"

void DeferredSpotLightRenderPass::Init (const RenderSettings& settings)
{
	/*
	 * Initialize volumetric light render pass
	*/

	VolumetricLightRenderPass::Init (settings);

	/*
	 * Shader for general directional light with no shadow casting
	*/

	Resource<Shader> shader = Resources::LoadShader ({
		"Assets/Shaders/deferredSpotVolLightVertex.glsl",
		"Assets/Shaders/deferredSpotVolLightFragment.glsl"
	});

	_shaderView = RenderSystem::LoadShader (shader);

	/*
	 * Shader for directional light with shadow casting
	*/

	Resource<Shader> shadowShader = Resources::LoadShader ({
		"Assets/Shaders/deferredSpotVolLightVertex.glsl",
		"Assets/Shaders/deferredSpotVolShadowMapLightFragment.glsl"
	});

	_shadowShaderView = RenderSystem::LoadShader (shadowShader);
}

void DeferredSpotLightRenderPass::Clear ()
{
	/*
	 * Nothing
	*/
}

void DeferredSpotLightRenderPass::LockShader (const RenderLightObject* renderLightObject)
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

	/*
	 * Lock general shader for directional light
	*/

	if (renderLightObject->IsCastingShadows () == false) {
		Pipeline::LockShader (_shaderView);
	}
}

std::vector<PipelineAttribute> DeferredSpotLightRenderPass::GetCustomAttributes (const RenderLightObject* renderLightObject) const
{
	auto renderSpotLightObject = dynamic_cast<const RenderSpotLightObject*> (renderLightObject);

	std::vector<PipelineAttribute> attributes;

	PipelineAttribute lightRange;
	PipelineAttribute lightSpotCutoff;
	PipelineAttribute lightSpotOuterCutoff;

	lightRange.type = PipelineAttribute::AttrType::ATTR_1F;
	lightSpotCutoff.type = PipelineAttribute::AttrType::ATTR_1F;
	lightSpotOuterCutoff.type = PipelineAttribute::AttrType::ATTR_1F;

	lightRange.name = "lightRange";
	lightSpotCutoff.name = "lightSpotCutoff";
	lightSpotOuterCutoff.name = "lightSpotOuterCutoff";

	lightRange.value.x = renderSpotLightObject->GetLightRange ();
	lightSpotCutoff.value.x = std::cos (DEG2RAD * renderSpotLightObject->GetLightSpotCutoff ());
	lightSpotOuterCutoff.value.x = std::cos (DEG2RAD * renderSpotLightObject->GetLightSpotOuterCutoff ());

	attributes.push_back (lightRange);
	attributes.push_back (lightSpotCutoff);
	attributes.push_back (lightSpotOuterCutoff);

	return attributes;
}
