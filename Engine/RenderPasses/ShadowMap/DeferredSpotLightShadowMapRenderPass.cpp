#include "DeferredSpotLightShadowMapRenderPass.h"

#include "Managers/ShaderManager.h"

#include "Renderer/Pipeline.h"

#include "SceneNodes/SceneLayer.h"

DeferredSpotLightShadowMapRenderPass::DeferredSpotLightShadowMapRenderPass () :
	_staticShaderName ("STATIC_SPOT_LIGHT_SHADOW_MAP"),
	_animationShaderName ("ANIMATION_SPOT_LIGHT_SHADOW_MAP")
{

}

void DeferredSpotLightShadowMapRenderPass::Init (const RenderSettings& settings)
{
	/*
	 * Shader for animated objects
	*/

	ShaderManager::Instance ()->AddShader (_staticShaderName,
		"Assets/Shaders/ShadowMap/shadowMapVertex.glsl",
		"Assets/Shaders/ShadowMap/shadowMapFragment.glsl");

	/*
	 * Shader for animated objects
	*/

	ShaderManager::Instance ()->AddShader (_animationShaderName,
		"Assets/Shaders/ShadowMap/shadowMapVertexAnimation.glsl",
		"Assets/Shaders/ShadowMap/shadowMapFragment.glsl");
}

void DeferredSpotLightShadowMapRenderPass::LockShader (int sceneLayers)
{
	/*
	 * Unlock last shader
	*/

	Pipeline::UnlockShader ();

	/*
	 * Lock the shader for animations
	*/

	if (sceneLayers & SceneLayer::ANIMATION) {
		Pipeline::LockShader (ShaderManager::Instance ()->GetShader (_animationShaderName));
	}

	/*
	 * Lock general shader for not animated objects
	*/

	if (sceneLayers & (SceneLayer::STATIC | SceneLayer::DYNAMIC)) {
		Pipeline::LockShader (ShaderManager::Instance ()->GetShader (_staticShaderName));
	}
}
