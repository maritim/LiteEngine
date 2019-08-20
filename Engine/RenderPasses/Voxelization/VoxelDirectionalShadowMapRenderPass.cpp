#include "VoxelDirectionalShadowMapRenderPass.h"

#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/quaternion.hpp>
#include <algorithm>

#include "Managers/ShaderManager.h"
#include "Cameras/OrthographicCamera.h"

#include "Renderer/Pipeline.h"

#include "Core/Intersections/Intersection.h"

#include "Core/Console/Console.h"

#include "SceneNodes/SceneLayer.h"

VoxelDirectionalShadowMapRenderPass::VoxelDirectionalShadowMapRenderPass () :
	_staticShaderName ("STATIC_SHADOW_MAP"),
	_animationShaderName ("ANIMATION_SHADOW_MAP"),
	_voxelShadowMapVolume (new VoxelShadowMapVolume ()),
	_firstTime (true)
{

}

VoxelDirectionalShadowMapRenderPass::~VoxelDirectionalShadowMapRenderPass ()
{
	delete _voxelShadowMapVolume;
}

void VoxelDirectionalShadowMapRenderPass::Init (const RenderSettings& settings)
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

	//TODO: Change this
	if (!_voxelShadowMapVolume->Init (1, glm::ivec2 (1024))) {
		Console::LogError ("Shadow map cannot be initialized! It is not possible to continue the process. End now!");
		exit (SHADOW_MAP_FBO_NOT_INIT);
	}
}

RenderVolumeCollection* VoxelDirectionalShadowMapRenderPass::Execute (const RenderScene* renderScene, const Camera* camera,
	const RenderSettings& settings, RenderVolumeCollection* rvc)
{
	if (_firstTime || settings.vct_continuous_voxelization) {

		/*
		* Start shadow map drawing process
		*/

		StartShadowMapPass ();

		/*
		* Calculate light camera for shadow map
		*/

		Camera* lightCamera = GetLightCamera (renderScene, camera);
		_voxelShadowMapVolume->SetLightCamera (lightCamera);

		/*
		* Render geometry on shadow map
		*/

		ShadowMapGeometryPass (renderScene, lightCamera);

		/*
		* End shadow map drawing process
		*/

		EndShadowMapPass ();

		_firstTime = false;
	}

	return rvc->Insert ("ShadowMapDirectionalLightVolume", _voxelShadowMapVolume);
}

void VoxelDirectionalShadowMapRenderPass::Clear ()
{
	/*
	 * Clear voxel shadow map volume
	*/

	_voxelShadowMapVolume->Clear ();
}

void VoxelDirectionalShadowMapRenderPass::StartShadowMapPass ()
{
	/*
	* Bind shadow map volume for writing
	*/

	_voxelShadowMapVolume->BindForWriting ();
}

void VoxelDirectionalShadowMapRenderPass::ShadowMapGeometryPass (const RenderScene* renderScene, Camera* lightCamera)
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

	for (RenderObject* renderObject : *renderScene) {

		/*
		 * Check if it's active
		*/

		if (renderObject->IsActive () == false) {
			continue;
		}

		if (renderObject->GetRenderStage () != RenderStage::RENDER_STAGE_DEFERRED) {
			continue;
		}

		/*
		* Culling Check
		*/

		if (renderObject->GetCollider () != nullptr) {
			GeometricPrimitive* primitive = renderObject->GetCollider ()->GetGeometricPrimitive ();
			if (!Intersection::Instance ()->CheckFrustumVsPrimitive (frustum, primitive)) {
				continue;
			}
		}

		/*
		* Lock shader based on scene object layer
		*/

		LockShader (renderObject->GetSceneLayers ());

		/*
		* Render object on shadow map
		*/

		renderObject->Draw ();
	}
}

void VoxelDirectionalShadowMapRenderPass::EndShadowMapPass ()
{
	/*
	 * Unlock shader
	*/

	Pipeline::UnlockShader ();
}

Camera* VoxelDirectionalShadowMapRenderPass::GetLightCamera (const RenderScene* renderScene, const Camera* viewCamera)
{
	const float LIGHT_CAMERA_OFFSET = 50.0f;

	auto renderLightObject = renderScene->GetRenderDirectionalLightObject ();

	if (renderLightObject == nullptr) {
		Console::LogError ("There is no active directional light to render. End now!");
		exit (230);
	}

	Transform* lightTransform = renderLightObject->GetTransform ();

	glm::vec3 lightDir = glm::normalize (lightTransform->GetPosition ()) * -1.0f;
	glm::quat lightDirQuat = glm::toQuat (glm::lookAt (glm::vec3 (0), lightDir, glm::vec3 (0, 1, 0)));
	glm::mat4 lightView = glm::translate (glm::mat4_cast (lightDirQuat), glm::vec3 (0));

	glm::vec3 cuboidExtendsMin = glm::vec3 (std::numeric_limits<float>::max ());
	glm::vec3 cuboidExtendsMax = glm::vec3 (-std::numeric_limits<float>::min ());

	AABBVolume* aabbVolume = renderScene->GetBoundingBox ();
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

void VoxelDirectionalShadowMapRenderPass::LockShader (int sceneLayers)
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
