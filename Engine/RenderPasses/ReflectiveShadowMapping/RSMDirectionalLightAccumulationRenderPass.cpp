#include "RSMDirectionalLightAccumulationRenderPass.h"

#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/quaternion.hpp>
#include <algorithm>

#include "Managers/ShaderManager.h"
#include "Cameras/OrthographicCamera.h"

#include "Renderer/Pipeline.h"

#include "Core/Intersections/Intersection.h"

#include "Core/Console/Console.h"

#include "SceneNodes/SceneLayer.h"

#include "Debug/Statistics/StatisticsManager.h"
#include "Debug/Statistics/RSMStatisticsObject.h"

RSMDirectionalLightAccumulationRenderPass::RSMDirectionalLightAccumulationRenderPass () :
	_staticShaderName ("STATIC_REFLECTIVE_SHADOW_MAP"),
	_animationShaderName ("ANIMATION_REFLECTIVE_SHADOW_MAP"),
	_reflectiveShadowMapVolume (new RSMVolume ())
{

}

RSMDirectionalLightAccumulationRenderPass::~RSMDirectionalLightAccumulationRenderPass ()
{
	delete _reflectiveShadowMapVolume;
}

void RSMDirectionalLightAccumulationRenderPass::Init (const RenderSettings& settings)
{
	/*
	 * Shader for static objects
	*/

	ShaderManager::Instance ()->AddShader (_staticShaderName,
		"Assets/Shaders/ReflectiveShadowMapping/reflectiveShadowMapVertex.glsl",
		"Assets/Shaders/ReflectiveShadowMapping/reflectiveShadowMapFragment.glsl");

	/*
	 * Shader for animated objects
	*/

	ShaderManager::Instance ()->AddShader (_animationShaderName,
		"Assets/Shaders/ReflectiveShadowMapping/reflectiveShadowMapVertexAnimation.glsl",
		"Assets/Shaders/ReflectiveShadowMapping/reflectiveShadowMapFragment.glsl");

	/*
	 * Initialize reflective shadow map volume
	*/

	InitRSMVolume (settings);
}

RenderVolumeCollection* RSMDirectionalLightAccumulationRenderPass::Execute (const RenderScene* renderScene, const Camera* camera,
	const RenderSettings& settings, RenderVolumeCollection* rvc)
{
	/*
	 * Update volumes
	*/

	UpdateRSMVolume (settings);

	/*
	* Start shadow map drawing process
	*/

	StartShadowMapPass ();

	/*
	* Calculate light camera for shadow map
	*/

	Camera* lightCamera = GetLightCamera (renderScene, camera);
	_reflectiveShadowMapVolume->SetLightCamera (lightCamera);

	/*
	* Render geometry on shadow map
	*/

	ShadowMapGeometryPass (renderScene, lightCamera, settings);

	/*
	* End shadow map drawing process
	*/

	EndShadowMapPass ();

	return rvc->Insert ("ReflectiveShadowMapVolume", _reflectiveShadowMapVolume);
}

bool RSMDirectionalLightAccumulationRenderPass::IsAvailable (const RenderLightObject* renderLightObject) const
{
	/*
	 * Execute reflective shadow map accumulation sub pass only if light is casting shadows
	*/

	return renderLightObject->IsCastingShadows ();
}

void RSMDirectionalLightAccumulationRenderPass::Clear ()
{
	_reflectiveShadowMapVolume->Clear ();
}

void RSMDirectionalLightAccumulationRenderPass::StartShadowMapPass ()
{
	/*
	* Bind shadow map volume for writing
	*/

	_reflectiveShadowMapVolume->BindForWriting ();
}

void RSMDirectionalLightAccumulationRenderPass::ShadowMapGeometryPass (const RenderScene* renderScene, const Camera* lightCamera,
	const RenderSettings& settings)
{
	/*
	* Send light camera
	*/

	Pipeline::CreateProjection (lightCamera->GetProjectionMatrix ());
	Pipeline::SendCamera (lightCamera);

	/*
	 * Clear framebuffer
	*/

	GL::Clear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

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
	* Light camera
	*/

	FrustumVolume* frustum = lightCamera->GetFrustumVolume ();

	/*
	* Render scene entities to framebuffer at Deferred Rendering Stage
	*/

	for_each_type (RenderObject*, renderObject, *renderScene) {

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
		* Lock shader based on scene object layers
		*/

		LockShader (renderObject->GetSceneLayers ());

		/*
		* Render object on shadow map
		*/

		renderObject->Draw ();
	}
}

void RSMDirectionalLightAccumulationRenderPass::EndShadowMapPass ()
{
	_reflectiveShadowMapVolume->EndDrawing ();
}

Camera* RSMDirectionalLightAccumulationRenderPass::GetLightCamera (const RenderScene* renderScene, const Camera* viewCamera)
{
	const float LIGHT_CAMERA_OFFSET = 50.0f;

	auto renderLightObject = renderScene->GetRenderDirectionalLightObject ();

	if (renderLightObject == nullptr) {
		Console::LogError ("There is no active directional light to render. End now!");
		exit (230);
	}

	Transform* lightTransform = renderLightObject->GetTransform ();

	glm::vec3 lightDir = glm::normalize(lightTransform->GetPosition()) * -1.0f;
	glm::quat lightDirQuat = glm::toQuat(glm::lookAt(glm::vec3(0), lightDir, glm::vec3(0, 1, 0)));
	glm::mat4 lightView = glm::translate(glm::mat4_cast(lightDirQuat), glm::vec3(0));

	glm::vec3 cuboidExtendsMin = glm::vec3(std::numeric_limits<float>::max());
	glm::vec3 cuboidExtendsMax = glm::vec3(-std::numeric_limits<float>::min());

	AABBVolume* aabbVolume = renderScene->GetBoundingBox();
	AABBVolume::AABBVolumeInformation* bBox = aabbVolume->GetVolumeInformation();

	OrthographicCamera* lightCamera = new OrthographicCamera();

	for (int x = 0; x <= 1; x++) {
		for (int y = 0; y <= 1; y++) {
			for (int z = 0; z <= 1; z++) {
				glm::vec4 cuboidCorner = glm::vec4(
					x == 0 ? bBox->minVertex.x : bBox->maxVertex.x,
					y == 0 ? bBox->minVertex.y : bBox->maxVertex.y,
					z == 0 ? bBox->minVertex.z : bBox->maxVertex.z,
					1.0f
				);

				cuboidCorner = lightView * cuboidCorner;

				cuboidExtendsMin.x = std::min(cuboidExtendsMin.x, cuboidCorner.x);
				cuboidExtendsMin.y = std::min(cuboidExtendsMin.y, cuboidCorner.y);
				cuboidExtendsMin.z = std::min(cuboidExtendsMin.z, cuboidCorner.z);

				cuboidExtendsMax.x = std::max(cuboidExtendsMax.x, cuboidCorner.x);
				cuboidExtendsMax.y = std::max(cuboidExtendsMax.y, cuboidCorner.y);
				cuboidExtendsMax.z = std::max(cuboidExtendsMax.z, cuboidCorner.z);
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
		Pipeline::LockShader (ShaderManager::Instance ()->GetShader (_animationShaderName));
	}

	/*
	 * Lock general shader for not animated objects
	*/

	if (sceneLayers & (SceneLayer::STATIC | SceneLayer::DYNAMIC)) {
		Pipeline::LockShader (ShaderManager::Instance ()->GetShader (_staticShaderName));
	}
}

void RSMDirectionalLightAccumulationRenderPass::InitRSMVolume (const RenderSettings& settings)
{
	if (!_reflectiveShadowMapVolume->Init (settings.rsm_resolution, 1)) {
		Console::LogError (std::string () + "Reflective shadow map cannot be initialized!" +
			"It is not possible to continue the process. End now!");
		exit (REFLECTIVE_SHADOW_MAP_FBO_NOT_INIT);
	}
}

void RSMDirectionalLightAccumulationRenderPass::UpdateRSMVolume (const RenderSettings& settings)
{
	glm::ivec2 rsmSize = _reflectiveShadowMapVolume->GetSize ();

	if (rsmSize != settings.rsm_resolution) {

		/*
		 * Clear framebuffer
		*/

		_reflectiveShadowMapVolume->Clear ();

		/*
		 * Initialize framebuffer
		*/

		InitRSMVolume (settings);
	}

	StatisticsObject* stat = StatisticsManager::Instance ()->GetStatisticsObject ("RSMStatisticsObject");
	RSMStatisticsObject* rsmStatisticsObject = nullptr;

	if (stat == nullptr) {
		stat = new RSMStatisticsObject ();
		StatisticsManager::Instance ()->SetStatisticsObject ("RSMStatisticsObject", stat);
	}

	rsmStatisticsObject = dynamic_cast<RSMStatisticsObject*> (stat);

	rsmStatisticsObject->rsmPosMapID = _reflectiveShadowMapVolume->GetCascade (0)->GetColorBuffer (0);
	rsmStatisticsObject->rsmNormalMapID = _reflectiveShadowMapVolume->GetCascade (0)->GetColorBuffer (1);
	rsmStatisticsObject->rsmFluxMapID = _reflectiveShadowMapVolume->GetCascade (0)->GetColorBuffer (2);
	rsmStatisticsObject->rsmDepthMapID = _reflectiveShadowMapVolume->GetCascade (0)->GetDepthBuffer ();
}