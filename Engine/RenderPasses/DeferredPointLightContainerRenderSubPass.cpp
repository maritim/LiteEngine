#include "DeferredPointLightContainerRenderSubPass.h"

#include "Managers/ShaderManager.h"

#include "Renderer/Pipeline.h"

DeferredPointLightContainerRenderSubPass::DeferredPointLightContainerRenderSubPass () :
	_shaderName ("POINT_LIGHT"),
	_shadowShaderName ("SHADOW_MAP_POINT_LIGHT")
{

}

void DeferredPointLightContainerRenderSubPass::Init ()
{
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

void DeferredPointLightContainerRenderSubPass::LockShader (const VolumetricLight* volumetricLight)
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

	if (volumetricLight->IsCastingShadows () == false) {
		Pipeline::LockShader (ShaderManager::Instance ()->GetShader (_shaderName));
	}
}