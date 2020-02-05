#include "DeferredDirectionalLightRenderPass.h"

#include "Resources/Resources.h"
#include "Renderer/RenderSystem.h"

#include "Renderer/Pipeline.h"

void DeferredDirectionalLightRenderPass::Init (const RenderSettings& settings)
{
	/*
	 * Shader for general directional light with no shadow casting
	*/

	Resource<Shader> shader = Resources::LoadShader ({
		"Assets/Shaders/deferredDirVolLightVertex.glsl",
		"Assets/Shaders/deferredDirVolLightFragment.glsl"
	});

	_shaderView = RenderSystem::LoadShader (shader);

	/*
	 * Shader for directional light with shadow casting
	*/

	Resource<Shader> shadowShader = Resources::LoadShader ({
		"Assets/Shaders/deferredDirVolLightVertex.glsl",
		"Assets/Shaders/deferredDirVolShadowMapLightFragment.glsl"
	});

	_shadowShaderView = RenderSystem::LoadShader (shadowShader);
}

void DeferredDirectionalLightRenderPass::Clear ()
{
	/*
	 * Nothing
	*/
}

void DeferredDirectionalLightRenderPass::LockShader (const RenderLightObject* renderLightObject)
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

std::vector<PipelineAttribute> DeferredDirectionalLightRenderPass::GetCustomAttributes (const RenderSettings& settings) const
{
	std::vector<PipelineAttribute> attributes;

	return attributes;
}
