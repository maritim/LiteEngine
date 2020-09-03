#include "RSMDirectionalLightViewAccumulationRenderPass.h"

#include "Resources/Resources.h"
#include "Renderer/RenderSystem.h"

void RSMDirectionalLightViewAccumulationRenderPass::Init (const RenderSettings& settings)
{
	/*
	 * Shader for static objects
	*/

	Resource<Shader> staticShader = Resources::LoadShader ({
		"Assets/Shaders/ReflectiveShadowMapping/reflectiveShadowMapViewAccumulationVertex.glsl",
		"Assets/Shaders/ReflectiveShadowMapping/reflectiveShadowMapAccumulationFragment.glsl"
	});

	_staticShaderView = RenderSystem::LoadShader (staticShader);

	/*
	 * Shader for animated objects
	*/

	Resource<Shader> animationShader = Resources::LoadShader ({
		"Assets/Shaders/ReflectiveShadowMapping/reflectiveShadowMapViewAccumulationVertexAnimation.glsl",
		"Assets/Shaders/ReflectiveShadowMapping/reflectiveShadowMapAccumulationFragment.glsl"
	});

	_animationShaderView = RenderSystem::LoadShader (animationShader);
}
