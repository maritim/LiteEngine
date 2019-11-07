#include "DeferredSpotLightRenderPass.h"

#include "Managers/ShaderManager.h"

#include "Renderer/Pipeline.h"

DeferredSpotLightRenderPass::DeferredSpotLightRenderPass () :
	_shaderName ("SPOT_LIGHT"),
	_shadowShaderName ("SHADOW_MAP_SPOT_LIGHT")
{

}

void DeferredSpotLightRenderPass::Init (const RenderSettings& settings)
{
	/*
	 * Shader for general directional light with no shadow casting
	*/

	ShaderManager::Instance ()->AddShader (_shaderName,
		"Assets/Shaders/deferredSpotVolLightVertex.glsl",
		"Assets/Shaders/deferredSpotVolLightFragment.glsl");

	/*
	 * Shader for directional light with shadow casting
	*/

	// ShaderManager::Instance ()->AddShader (_shadowShaderName,
	// 	"Assets/Shaders/ShadowMap/deferredDirVolShadowMapLightVertex.glsl",
	// 	"Assets/Shaders/ShadowMap/deferredDirVolShadowMapLightFragment.glsl");
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