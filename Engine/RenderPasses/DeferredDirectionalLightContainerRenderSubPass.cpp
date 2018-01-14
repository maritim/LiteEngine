#include "DeferredDirectionalLightContainerRenderSubPass.h"

#include "Managers/ShaderManager.h"

#include "Renderer/Pipeline.h"

DeferredDirectionalLightContainerRenderSubPass::DeferredDirectionalLightContainerRenderSubPass () :
	_shaderName ("DIRECTIONAL_LIGHT"),
	_shadowShaderName ("SHADOW_MAP_DIRECTIONAL_LIGHT")
{

}

void DeferredDirectionalLightContainerRenderSubPass::Init ()
{
	/*
	 * Shader for general directional light with no shadow casting
	*/

	ShaderManager::Instance ()->AddShader (_shaderName,
		"Assets/Shaders/deferredDirVolLightVertex.glsl",
		"Assets/Shaders/deferredDirVolLightFragment.glsl");

	/*
	 * Shader for directional light with shadow casting
	*/

	ShaderManager::Instance ()->AddShader (_shadowShaderName,
		"Assets/Shaders/ShadowMap/deferredDirVolShadowMapLightVertex.glsl",
		"Assets/Shaders/ShadowMap/deferredDirVolShadowMapLightFragment.glsl");
}

void DeferredDirectionalLightContainerRenderSubPass::LockShader (const VolumetricLight* volumetricLight)
{
	/*
	 * Unlock last shader
	*/

	Pipeline::UnlockShader ();

	/*
	 * Lock shader for shadow directional light
	*/

	if (volumetricLight->IsCastingShadows () == true) {
		Pipeline::LockShader (ShaderManager::Instance ()->GetShader (_shadowShaderName));
	}

	/*
	 * Lock general shader for directional light
	*/

	if (volumetricLight->IsCastingShadows () == false) {
		Pipeline::LockShader (ShaderManager::Instance ()->GetShader (_shaderName));
	}
}