#include "LightMapDirectionalLightRenderPass.h"

#include "Resources/Resources.h"
#include "Renderer/RenderSystem.h"

#include "Renderer/Pipeline.h"

void LightMapDirectionalLightRenderPass::Init (const RenderSettings& settings)
{
	/*
	 * Shader for general directional light with no shadow casting
	*/

	Resource<Shader> shader = Resources::LoadShader ({
		"Assets/Shaders/deferredDirVolLightVertex.glsl",
		"Assets/Shaders/LightMap/lightMapDirVolLightFragment.glsl"
	});

	_shaderView = RenderSystem::LoadShader (shader);
}

void LightMapDirectionalLightRenderPass::Clear ()
{
	/*
	 * Nothing
	*/
}

void LightMapDirectionalLightRenderPass::LockShader (const RenderLightObject* renderLightObject)
{
	/*
	 * Unlock last shader
	*/

	Pipeline::UnlockShader ();

	/*
	 * Lock general shader for directional light
	*/

	Pipeline::LockShader (_shaderView);
}

std::vector<PipelineAttribute> LightMapDirectionalLightRenderPass::GetCustomAttributes (const RenderSettings& settings) const
{
	std::vector<PipelineAttribute> attributes;

	return attributes;
}
