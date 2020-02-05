#include "RSMDirectionalLightRenderPass.h"

#include "Resources/Resources.h"
#include "Renderer/RenderSystem.h"

#include "Renderer/Pipeline.h"

void RSMDirectionalLightRenderPass::Init (const RenderSettings& settings)
{
	/*
	 * Shader for directional light with shadow casting
	*/

	Resource<Shader> shadowShader = Resources::LoadShader ({
		"Assets/Shaders/ReflectiveShadowMapping/reflectiveDeferredDirVolShadowMapLightVertex.glsl",
		"Assets/Shaders/ReflectiveShadowMapping/reflectiveDeferredDirVolShadowMapLightFragment.glsl"
	});

	_shadowShaderView = RenderSystem::LoadShader (shadowShader);
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

	Pipeline::LockShader (_shadowShaderView);
}

std::vector<PipelineAttribute> RSMDirectionalLightRenderPass::GetCustomAttributes (const RenderSettings& settings) const
{
	std::vector<PipelineAttribute> attributes;

	return attributes;
}
