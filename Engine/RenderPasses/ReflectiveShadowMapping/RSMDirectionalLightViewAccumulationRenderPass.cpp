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

void RSMDirectionalLightViewAccumulationRenderPass::StartShadowMapPass (const Camera* lightCamera)
{
	/*
	 * Clear all buffers
	*/

	RSMDirectionalLightAccumulationRenderPass::StartShadowMapPass (lightCamera);

	/*
	 * Clear position buffer
	*/

	GL::DrawBuffer (GL_COLOR_ATTACHMENT0);

	GL::ClearColor (0, 0, -lightCamera->GetZFar () - 1, 0);
	GL::Clear (GL_COLOR_BUFFER_BIT);

	/*
	* Bind shadow map volume for writing
	*/

	_rsmVolume->GetFramebufferView ()->Activate ();
}
