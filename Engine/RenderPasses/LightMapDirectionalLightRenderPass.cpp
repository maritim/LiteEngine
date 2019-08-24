#include "LightMapDirectionalLightRenderPass.h"

#include "Managers/ShaderManager.h"

#include "Renderer/Pipeline.h"

LightMapDirectionalLightRenderPass::LightMapDirectionalLightRenderPass () :
	_shaderName ("LIGHT_MAP_DIRECTIONAL_LIGHT")
{

}

void LightMapDirectionalLightRenderPass::Init (const RenderSettings& settings)
{
	/*
	 * Shader for general directional light with no shadow casting
	*/

	ShaderManager::Instance ()->AddShader (_shaderName,
		"Assets/Shaders/deferredDirVolLightVertex.glsl",
		"Assets/Shaders/LightMap/lightMapDirVolLightFragment.glsl");
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

	Pipeline::LockShader (ShaderManager::Instance ()->GetShader (_shaderName));
}

std::vector<PipelineAttribute> LightMapDirectionalLightRenderPass::GetCustomAttributes (const RenderSettings& settings) const
{
	std::vector<PipelineAttribute> attributes;

	return attributes;
}
