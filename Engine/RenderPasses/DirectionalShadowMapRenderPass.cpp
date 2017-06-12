#include "DirectionalShadowMapRenderPass.h"

#include <algorithm>

#include "Core/Math/glm/gtx/quaternion.hpp"
#include "Core/Math/glm/gtc/quaternion.hpp"

#include "Lighting/LightsManager.h"
#include "Cameras/OrthographicCamera.h"

#include "Renderer/Pipeline.h"

#include "Core/Intersections/Intersection.h"

#include "Core/Console/Console.h"

DirectionalShadowMapRenderPass::DirectionalShadowMapRenderPass () :
	_voxelShadowMapVolume (new VoxelShadowMapVolume ())
{

}

DirectionalShadowMapRenderPass::~DirectionalShadowMapRenderPass ()
{
	delete _voxelShadowMapVolume;
}

void DirectionalShadowMapRenderPass::Init ()
{
	if (!_voxelShadowMapVolume->Init (1)) {
		Console::LogError ("Shadow map cannot be initialized! It is not possible to continue the process. End now!");
		exit (SHADOW_MAP_FBO_NOT_INIT);
	}
}

RenderVolumeCollection* DirectionalShadowMapRenderPass::Execute (Scene* scene, Camera* camera, RenderVolumeCollection* rvc)
{
	/*
	* Start shadow map drawing process
	*/

	StartShadowMapPass ();

	/*
	* Calculate light camera for shadow map
	*/

	Camera* lightCamera = GetLightCamera (camera);
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

void DirectionalShadowMapRenderPass::ShadowMapGeometryPass (Scene* scene, Camera* lightCamera)
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

		_voxelShadowMapVolume->LockShader (sceneObject->GetLayers ());

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

Camera* DirectionalShadowMapRenderPass::GetLightCamera (Camera* viewCamera)
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

	glm::mat4 cameraView = glm::translate (glm::mat4_cast (viewCamera->GetRotation ()), viewCamera->GetPosition () * -1.0f);
	glm::mat4 cameraProjection = viewCamera->GetProjectionMatrix ();
	glm::mat4 invCameraProjView = glm::inverse (cameraProjection * cameraView);

	glm::vec3 cuboidExtendsMin = glm::vec3 (std::numeric_limits<float>::max ());
	glm::vec3 cuboidExtendsMax = glm::vec3 (-std::numeric_limits<float>::min ());

	OrthographicCamera* lightCamera = new OrthographicCamera ();

	for (int x = -1; x <= 1; x += 2) {
		for (int y = -1; y <= 1; y += 2) {
			for (int z = -1; z <= 1; z += 2) {
				glm::vec4 cuboidCorner = glm::vec4 (x, y, z, 1.0f);

				cuboidCorner = invCameraProjView * cuboidCorner;
				cuboidCorner /= cuboidCorner.w;

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
		cuboidExtendsMin.x - LIGHT_CAMERA_OFFSET, cuboidExtendsMax.x + LIGHT_CAMERA_OFFSET,
		cuboidExtendsMin.y - LIGHT_CAMERA_OFFSET, cuboidExtendsMax.y + LIGHT_CAMERA_OFFSET,
		cuboidExtendsMin.z - LIGHT_CAMERA_OFFSET, cuboidExtendsMax.z + LIGHT_CAMERA_OFFSET
	);

	return lightCamera;
}

