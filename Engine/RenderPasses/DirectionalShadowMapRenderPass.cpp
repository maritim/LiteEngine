#include "DirectionalShadowMapRenderPass.h"

#include <algorithm>

#include "Core/Math/glm/gtx/quaternion.hpp"
#include "Core/Math/glm/gtc/quaternion.hpp"

#include "Managers/ShaderManager.h"
#include "Lighting/LightsManager.h"
#include "Cameras/OrthographicCamera.h"

#include "Renderer/Pipeline.h"

#include "Core/Intersections/Intersection.h"

#include "Core/Console/Console.h"

#include "SceneNodes/SceneLayer.h"

DirectionalShadowMapRenderPass::DirectionalShadowMapRenderPass () :
	_staticShaderName ("STATIC_SHADOW_MAP"),
	_animationShaderName ("ANIMATION_SHADOW_MAP"),
	_voxelShadowMapVolume (new VoxelShadowMapVolume ())
{

}

DirectionalShadowMapRenderPass::~DirectionalShadowMapRenderPass ()
{
	delete _voxelShadowMapVolume;
}

void DirectionalShadowMapRenderPass::Init ()
{
	/*
	 * Shader for static objects
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

	/*
	 * Initialize shadow map volume
	*/

	if (!_voxelShadowMapVolume->Init (1)) {
		Console::LogError ("Shadow map cannot be initialized! It is not possible to continue the process. End now!");
		exit (SHADOW_MAP_FBO_NOT_INIT);
	}
}

RenderVolumeCollection* DirectionalShadowMapRenderPass::Execute (const Scene* scene, const Camera* camera, RenderVolumeCollection* rvc)
{
	/*
	* Start shadow map drawing process
	*/

	StartShadowMapPass ();

	/*
	* Calculate light camera for shadow map
	*/

	Camera* lightCamera = GetLightCamera (scene, camera);
	_voxelShadowMapVolume->SetLightCamera (lightCamera);

	/*
	* Render geometry on shadow map
	*/

	ShadowMapGeometryPass (scene, lightCamera);

	/*
	* End shadow map drawing process
	*/

	EndShadowMapPass ();

	return rvc->Insert ("ShadowMapVolume", _voxelShadowMapVolume);
}

void DirectionalShadowMapRenderPass::StartShadowMapPass ()
{
	/*
	* Bind shadow map volume for writing
	*/

	_voxelShadowMapVolume->BindForWriting ();
}

void DirectionalShadowMapRenderPass::ShadowMapGeometryPass (const Scene* scene, Camera* lightCamera)
{
	/*
	* Send light camera
	*/

	Pipeline::CreateProjection (lightCamera->GetProjectionMatrix ());
	Pipeline::SendCamera (lightCamera);

	/*
	* Shadow map is a depth test
	*/

	GL::Enable (GL_DEPTH_TEST);
	GL::DepthMask (GL_TRUE);

	/*
	* Doesn't really matter
	*/

	GL::Enable (GL_BLEND);
	GL::BlendFunc (GL_ONE, GL_ZERO);

	/*
	* Enable front face culling
	*/

	GL::Enable (GL_CULL_FACE);
	GL::CullFace (GL_FRONT);

	/*
	* Light camera
	*/

	FrustumVolume* frustum = lightCamera->GetFrustumVolume ();

	/*
	* Render scene entities to framebuffer at Deferred Rendering Stage
	*/

	for (SceneObject* sceneObject : *scene) {
		if (sceneObject->GetRenderer ()->GetStageType () != Renderer::StageType::DEFERRED_STAGE) {
			continue;
		}

		/*
		* Culling Check
		*/

		if (sceneObject->GetCollider () == nullptr) {
			continue;
		}

		GeometricPrimitive* primitive = sceneObject->GetCollider ()->GetGeometricPrimitive ();
		if (!Intersection::Instance ()->CheckFrustumVsPrimitive (frustum, primitive)) {
			continue;
		}

		/*
		* Lock shader based on scene object layer
		*/

		LockShader (sceneObject->GetLayers ());

		/*
		* Render object on shadow map
		*/

		sceneObject->GetRenderer ()->Draw ();
	}
}

void DirectionalShadowMapRenderPass::EndShadowMapPass ()
{
	_voxelShadowMapVolume->EndDrawing ();
}

Camera* DirectionalShadowMapRenderPass::GetLightCamera (const Scene* scene, const Camera* viewCamera)
{
	const float LIGHT_CAMERA_OFFSET = 50.0f;

	Light* dirLight = nullptr;

	for (std::size_t lightIndex = 0; lightIndex < LightsManager::Instance ()->GetDirectionalLightsCount (); lightIndex++) {
		dirLight = LightsManager::Instance ()->GetDirectionalLight (lightIndex);

		if (dirLight->IsActive ()) {
			break;
		}
	}

	if (dirLight == nullptr) {
		Console::LogError ("There is no active directional light to render. End now!");
		exit (230);
	}

	Transform* lightTransform = dirLight->GetTransform ();

	glm::vec3 lightDir = glm::normalize (lightTransform->GetPosition ()) * -1.0f;
	glm::quat lightDirQuat = glm::toQuat (glm::lookAt (glm::vec3 (0), lightDir, glm::vec3 (0, 1, 0)));
	glm::mat4 lightView = glm::translate (glm::mat4_cast (lightDirQuat), glm::vec3 (0));

	glm::vec3 cuboidExtendsMin = glm::vec3 (std::numeric_limits<float>::max ());
	glm::vec3 cuboidExtendsMax = glm::vec3 (-std::numeric_limits<float>::min ());

	AABBVolume* aabbVolume = scene->GetBoundingBox ();
	AABBVolume::AABBVolumeInformation* bBox = aabbVolume->GetVolumeInformation ();

	OrthographicCamera* lightCamera = new OrthographicCamera ();

	for (int x = 0; x <= 1; x ++) {
		for (int y = 0; y <= 1; y ++) {
			for (int z = 0; z <= 1; z ++) {
				glm::vec4 cuboidCorner = glm::vec4 (
					x == 0 ? bBox->minVertex.x : bBox->maxVertex.x,
					y == 0 ? bBox->minVertex.y : bBox->maxVertex.y,
					z == 0 ? bBox->minVertex.z : bBox->maxVertex.z,
					1.0f
				);

				cuboidCorner = lightView * cuboidCorner;

				cuboidExtendsMin.x = std::min (cuboidExtendsMin.x, cuboidCorner.x);
				cuboidExtendsMin.y = std::min (cuboidExtendsMin.y, cuboidCorner.y);
				cuboidExtendsMin.z = std::min (cuboidExtendsMin.z, cuboidCorner.z);

				cuboidExtendsMax.x = std::max (cuboidExtendsMax.x, cuboidCorner.x);
				cuboidExtendsMax.y = std::max (cuboidExtendsMax.y, cuboidCorner.y);
				cuboidExtendsMax.z = std::max (cuboidExtendsMax.z, cuboidCorner.z);
			}
		}
	}

	lightCamera->SetRotation (lightDirQuat);

	lightCamera->SetOrthographicInfo (
		cuboidExtendsMin.x, cuboidExtendsMax.x,
		cuboidExtendsMin.y, cuboidExtendsMax.y,
		cuboidExtendsMin.z - LIGHT_CAMERA_OFFSET, cuboidExtendsMax.z + LIGHT_CAMERA_OFFSET
	);

	return lightCamera;
}

void DirectionalShadowMapRenderPass::LockShader (int sceneLayers)
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
