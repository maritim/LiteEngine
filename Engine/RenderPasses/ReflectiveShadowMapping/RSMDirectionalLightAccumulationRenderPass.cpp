#include "RSMDirectionalLightAccumulationRenderPass.h"

#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/quaternion.hpp>
#include <algorithm>

#include "Cameras/OrthographicCamera.h"

#include "Resources/Resources.h"
#include "Renderer/RenderSystem.h"

#include "Renderer/Pipeline.h"

#include "SceneNodes/SceneLayer.h"

void RSMDirectionalLightAccumulationRenderPass::Init (const RenderSettings& settings)
{
	/*
	 * Shader for static objects
	*/

	Resource<Shader> staticShader = Resources::LoadShader ({
		"Assets/Shaders/ReflectiveShadowMapping/reflectiveShadowMapAccumulationVertex.glsl",
		"Assets/Shaders/ReflectiveShadowMapping/reflectiveShadowMapAccumulationFragment.glsl"
	});

	_staticShaderView = RenderSystem::LoadShader (staticShader);

	/*
	 * Shader for animated objects
	*/

	Resource<Shader> animationShader = Resources::LoadShader ({
		"Assets/Shaders/ReflectiveShadowMapping/reflectiveShadowMapAccumulationVertexAnimation.glsl",
		"Assets/Shaders/ReflectiveShadowMapping/reflectiveShadowMapAccumulationFragment.glsl"
	});

	_animationShaderView = RenderSystem::LoadShader (animationShader);
}

Camera* RSMDirectionalLightAccumulationRenderPass::GetLightCamera (const RenderScene* renderScene, const RenderLightObject* renderLightObject)
{
	const float LIGHT_CAMERA_OFFSET = 50.0f;

	glm::quat lightRotation = glm::conjugate (renderLightObject->GetTransform ()->GetRotation ());

	glm::vec3 cuboidExtendsMin = glm::vec3(std::numeric_limits<float>::max());
	glm::vec3 cuboidExtendsMax = glm::vec3(-std::numeric_limits<float>::min());

	auto& bBox = renderScene->GetBoundingBox ();

	OrthographicCamera* lightCamera = (OrthographicCamera*) _rsmVolume->GetLightCamera ();

	if (lightCamera == nullptr) {
		lightCamera = new OrthographicCamera ();
	}

	for (int x = 0; x <= 1; x++) {
		for (int y = 0; y <= 1; y++) {
			for (int z = 0; z <= 1; z++) {
				glm::vec3 cuboidCorner = glm::vec3(
					x == 0 ? bBox.minVertex.x : bBox.maxVertex.x,
					y == 0 ? bBox.minVertex.y : bBox.maxVertex.y,
					z == 0 ? bBox.minVertex.z : bBox.maxVertex.z
				);

				cuboidCorner = lightRotation * cuboidCorner;

				cuboidExtendsMin.x = std::min(cuboidExtendsMin.x, cuboidCorner.x);
				cuboidExtendsMin.y = std::min(cuboidExtendsMin.y, cuboidCorner.y);
				cuboidExtendsMin.z = std::min(cuboidExtendsMin.z, cuboidCorner.z);

				cuboidExtendsMax.x = std::max(cuboidExtendsMax.x, cuboidCorner.x);
				cuboidExtendsMax.y = std::max(cuboidExtendsMax.y, cuboidCorner.y);
				cuboidExtendsMax.z = std::max(cuboidExtendsMax.z, cuboidCorner.z);
			}
		}
	}

	lightCamera->SetRotation (lightRotation);

	lightCamera->SetOrthographicInfo (
		cuboidExtendsMin.x, cuboidExtendsMax.x,
		cuboidExtendsMin.y, cuboidExtendsMax.y,
		cuboidExtendsMin.z - LIGHT_CAMERA_OFFSET, cuboidExtendsMax.z + LIGHT_CAMERA_OFFSET
	);

	return lightCamera;
}

void RSMDirectionalLightAccumulationRenderPass::LockShader (int sceneLayers)
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
