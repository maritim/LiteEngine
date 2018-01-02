#include "ReflectiveShadowMapDirectionalVolumetricLightRenderPass.h"

#include "Managers/ShaderManager.h"

#include "Renderer/Pipeline.h"

ReflectiveShadowMapDirectionalVolumetricLightRenderPass::ReflectiveShadowMapDirectionalVolumetricLightRenderPass () :
	_shadowShaderName ("REFLECTIVE_SHADOW_MAP_DIRECTIONAL_LIGHT")
{

}

void ReflectiveShadowMapDirectionalVolumetricLightRenderPass::Init ()
{
	/*
	 * Shader for directional light with shadow casting
	*/

	ShaderManager::Instance ()->AddShader (_shadowShaderName,
		"Assets/Shaders/ReflectiveShadowMap/reflectiveDeferredDirVolShadowMapLightVertex.glsl",
		"Assets/Shaders/ReflectiveShadowMap/reflectiveDeferredDirVolShadowMapLightFragment.glsl");
}

void ReflectiveShadowMapDirectionalVolumetricLightRenderPass::LockShader (const VolumetricLight* volumetricLight)
{
	/*
	 * Unlock last shader
	*/

	Pipeline::UnlockShader ();

	/*
	 * Lock shader for shadow directional light
	*/

	Pipeline::LockShader (ShaderManager::Instance ()->GetShader (_shadowShaderName));
}