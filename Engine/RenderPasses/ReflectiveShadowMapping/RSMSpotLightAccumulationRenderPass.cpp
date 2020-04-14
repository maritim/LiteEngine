#include "RSMSpotLightAccumulationRenderPass.h"

#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/quaternion.hpp>
#include <algorithm>

#include "Cameras/PerspectiveCamera.h"

#include "Resources/Resources.h"
#include "Renderer/RenderSystem.h"

#include "Renderer/Pipeline.h"

#include "SceneNodes/SceneLayer.h"

#include "Utils/Extensions/MathExtend.h"

void RSMSpotLightAccumulationRenderPass::Init (const RenderSettings& settings)
{
	/*
	 * Shader for static objects
	*/

	Resource<Shader> staticShader = Resources::LoadShader ({
		"Assets/Shaders/ReflectiveShadowMapping/reflectiveShadowMapAccumulationVertex.glsl",
		"Assets/Shaders/ReflectiveShadowMapping/reflectiveShadowMapSpotLightAccumulationFragment.glsl"
	});

	_staticShaderView = RenderSystem::LoadShader (staticShader);

	/*
	 * Shader for animated objects
	*/

	Resource<Shader> animationShader = Resources::LoadShader ({
		"Assets/Shaders/ReflectiveShadowMapping/reflectiveShadowMapAccumulationVertexAnimation.glsl",
		"Assets/Shaders/ReflectiveShadowMapping/reflectiveShadowMapSpotLightAccumulationFragment.glsl"
	});

	_animationShaderView = RenderSystem::LoadShader (animationShader);
}

Camera* RSMSpotLightAccumulationRenderPass::GetLightCamera (const RenderScene* renderScene, const RenderLightObject* renderLightObject)
{
	auto renderSpotLightObject = dynamic_cast<const RenderSpotLightObject*> (renderLightObject);

	const Transform* lightTransform = renderSpotLightObject->GetTransform ();

	PerspectiveCamera* lightCamera = (PerspectiveCamera*) _reflectiveShadowMapVolume->GetLightCamera ();

	if (lightCamera == nullptr) {
		lightCamera = new PerspectiveCamera ();
	}

	lightCamera->SetPosition (lightTransform->GetPosition ());
	lightCamera->SetRotation (glm::conjugate (lightTransform->GetRotation ()));

	lightCamera->SetZFar (renderSpotLightObject->GetLightRange ());
	lightCamera->SetFieldOfView (renderSpotLightObject->GetLightSpotOuterCutoff () * 2);
	lightCamera->SetAspect (1.0f);

	return lightCamera;
}

void RSMSpotLightAccumulationRenderPass::LockShader (int sceneLayers)
{
	/*
	 * Unlock last shader
	*/

	Pipeline::UnlockShader ();

	/*
	 * Lock the shader for animations
	*/

	if (sceneLayers & SceneLayer::ANIMATION) {
		Pipeline::LockShader (_animationShaderView);
	}

	/*
	 * Lock general shader for not animated objects
	*/

	if (sceneLayers & (SceneLayer::STATIC | SceneLayer::DYNAMIC)) {
		Pipeline::LockShader (_staticShaderView);
	}
}

std::vector<PipelineAttribute> RSMSpotLightAccumulationRenderPass::GetCustomAttributes (const RenderLightObject* renderLightObject) const
{
	auto renderSpotLightObject = dynamic_cast<const RenderSpotLightObject*> (renderLightObject);

	std::vector<PipelineAttribute> attributes = RSMAccumulationRenderPass::GetCustomAttributes (renderLightObject);

	/*
	 * Add render spot light object
	*/

	PipelineAttribute lightRange;
	PipelineAttribute lightSpotCutoff;
	PipelineAttribute lightSpotOuterCutoff;

	lightRange.type = PipelineAttribute::AttrType::ATTR_1F;
	lightSpotCutoff.type = PipelineAttribute::AttrType::ATTR_1F;
	lightSpotOuterCutoff.type = PipelineAttribute::AttrType::ATTR_1F;

	lightRange.name = "lightRange";
	lightSpotCutoff.name = "lightSpotCutoff";
	lightSpotOuterCutoff.name = "lightSpotOuterCutoff";

	lightRange.value.x = renderSpotLightObject->GetLightRange ();
	lightSpotCutoff.value.x = std::cos (DEG2RAD * renderSpotLightObject->GetLightSpotCutoff ());
	lightSpotOuterCutoff.value.x = std::cos (DEG2RAD * renderSpotLightObject->GetLightSpotOuterCutoff ());

	attributes.push_back (lightRange);
	attributes.push_back (lightSpotCutoff);
	attributes.push_back (lightSpotOuterCutoff);

	return attributes;
}
