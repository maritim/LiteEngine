#include "LightMapDirectionalLightContainerRenderSubPass.h"

#include "Managers/ShaderManager.h"

#include "Renderer/Pipeline.h"

LightMapDirectionalLightContainerRenderSubPass::LightMapDirectionalLightContainerRenderSubPass () :
	_shaderName ("LIGHT_MAP_DIRECTIONAL_LIGHT")
{

}

void LightMapDirectionalLightContainerRenderSubPass::Init ()
{
	/*
	 * Shader for general directional light with no shadow casting
	*/

	ShaderManager::Instance ()->AddShader (_shaderName,
		"Assets/Shaders/deferredDirVolLightVertex.glsl",
		"Assets/Shaders/LightMap/lightMapDirVolLightFragment.glsl");
}

void LightMapDirectionalLightContainerRenderSubPass::LockShader (const VolumetricLight* volumetricLight)
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