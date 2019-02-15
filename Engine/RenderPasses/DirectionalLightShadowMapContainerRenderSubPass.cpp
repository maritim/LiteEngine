#include "DirectionalLightShadowMapContainerRenderSubPass.h"

#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <algorithm>

#include "Managers/ShaderManager.h"

#include "Core/Intersections/Intersection.h"

#include "Renderer/Pipeline.h"

#include "Core/Console/Console.h"

#include "SceneNodes/SceneLayer.h"

DirectionalLightShadowMapContainerRenderSubPass::DirectionalLightShadowMapContainerRenderSubPass () :
	_staticShaderName ("STATIC_SHADOW_MAP"),
	_animationShaderName ("ANIMATION_SHADOW_MAP"),
	_volume (new ShadowMapDirectionalLightVolume ())
{

}

DirectionalLightShadowMapContainerRenderSubPass::~DirectionalLightShadowMapContainerRenderSubPass ()
{
	delete _volume;
}

void DirectionalLightShadowMapContainerRenderSubPass::Init ()
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

	/*
	 * Initialize shadow map volume
	*/

	if (!_volume->Init (CASCADED_SHADOW_MAP_LEVELS)) {
		Console::LogError (std::string () + "Shadow map cannot be initialized!" +
			" It is not possible to continue the process. End now!");
		exit (SHADOW_MAP_FBO_NOT_INIT);
	}

	for (std::size_t index = 0; index < CASCADED_SHADOW_MAP_LEVELS; index ++) {
		_volume->SetLightCamera (index, new OrthographicCamera ());
	}
}

RenderVolumeCollection* DirectionalLightShadowMapContainerRenderSubPass::Execute (const Scene* scene, const Camera* camera, RenderVolumeCollection* rvc)
{
	/*
	 * Get volumetric light from render volume collection
	*/

	VolumetricLight* volumetricLight = GetVolumetricLight (rvc);

	/*
	 * Draw shadow map
	*/

	ShadowMapPass (scene, camera, volumetricLight);

	/*
	 * End drawing
	*/

	EndShadowMapPass ();

	return rvc->Insert ("ShadowMapDirectionalLightVolume", _volume);
}

void DirectionalLightShadowMapContainerRenderSubPass::Clear ()
{
	/*
	 * Clear shadow map volume
	*/

	_volume->Clear ();
}

bool DirectionalLightShadowMapContainerRenderSubPass::IsAvailable (const VolumetricLight* volumetricLight) const
{
	/*
	 * Execute shadow map sub pass only if light is casting shadows
	*/

	return volumetricLight->IsCastingShadows ();
}

void DirectionalLightShadowMapContainerRenderSubPass::ShadowMapPass (const Scene* scene, const Camera* camera, VolumetricLight* volumetricLight)
{
	UpdateCascadeLevelsLimits (camera);
	UpdateLightCameras (camera, volumetricLight);

	for (std::size_t index = 0; index < CASCADED_SHADOW_MAP_LEVELS; index++) {
		_volume->BindForShadowMapCatch (index);

		OrthographicCamera* lightCamera = (OrthographicCamera*) _volume->GetLightCamera (index);

		SendLightCamera (lightCamera);
		RenderScene (scene, lightCamera);
	}
}

void DirectionalLightShadowMapContainerRenderSubPass::EndShadowMapPass ()
{
	/*
	 * End shadow map pass
	*/

	_volume->EndDrawing ();	
}

void DirectionalLightShadowMapContainerRenderSubPass::UpdateCascadeLevelsLimits (const Camera* camera)
{
	/*
	 * TODO: Calculate this at run time
	*/

	_volume->SetCameraLimit (0, 0.92f);
	_volume->SetCameraLimit (1, 0.98f);
	_volume->SetCameraLimit (2, 0.995f);
	_volume->SetCameraLimit (3, 1.0f);
}

void DirectionalLightShadowMapContainerRenderSubPass::SendLightCamera (Camera* lightCamera)
{
	Pipeline::CreateProjection (lightCamera->GetProjectionMatrix ());
	Pipeline::SendCamera (lightCamera);
}

/*
 * Much thanks to: https://gamedev.stackexchange.com/questions/73851/how-do-i-fit-the-camera-frustum-inside-directional-light-space
 * Also thanks to: http://ogldev.atspace.co.uk/www/tutorial49/tutorial49.html
 *
 * 1. Calculate the 8 corners of the view frustum in world space. This can be 
 *    done by using the inverse view-projection matrix to transform the 8 
 *    corners of the NDC cube (which in OpenGL is [‒1, 1] along each axis).
 * 2. Transform the frustum corners to a space aligned with the shadow map 
 *    axes. This would commonly be the directional light object's local space. 
 * 3. Calculate the bounding box of the transformed frustum corners. This 
 *    will be the view frustum for the shadow map.
 * 4. Pass the bounding box's extents to glOrtho or similar to set up the 
 *    orthographic projection matrix for the shadow map.
*/

void DirectionalLightShadowMapContainerRenderSubPass::UpdateLightCameras (const Camera* viewCamera, VolumetricLight* volumetricLight)
{
	const float LIGHT_CAMERA_OFFSET = 100.0f;

	Transform* lightTransform = volumetricLight->GetTransform ();

	glm::vec3 lightDir = glm::normalize(lightTransform->GetPosition()) * -1.0f;
	glm::quat lightDirQuat = glm::toQuat(glm::lookAt(glm::vec3 (0), lightDir, glm::vec3(0, 1, 0)));
	glm::mat4 lightView = glm::translate (glm::mat4_cast (lightDirQuat), glm::vec3 (0));

	glm::mat4 cameraView = glm::translate(glm::mat4_cast(viewCamera->GetRotation()), viewCamera->GetPosition() * -1.0f);
	glm::mat4 cameraProjection = viewCamera->GetProjectionMatrix();
	glm::mat4 invCameraProjView = glm::inverse(cameraProjection * cameraView);

	for (std::size_t index = 0; index < CASCADED_SHADOW_MAP_LEVELS; index++) {

		OrthographicCamera* lightCamera = (OrthographicCamera*)_volume->GetLightCamera (index);

		glm::vec3 cuboidExtendsMin = glm::vec3(std::numeric_limits<float>::max());
		glm::vec3 cuboidExtendsMax = glm::vec3(-std::numeric_limits<float>::min());

		float zStart = index == 0 ? - 1 : _volume->GetCameraLimit (index - 1);
		float zEnd = _volume->GetCameraLimit (index);

		for (int x = -1; x <= 1; x += 2) {
			for (int y = -1; y <= 1; y += 2) {
				for (int z = -1; z <= 1; z += 2) {
					glm::vec4 cuboidCorner = glm::vec4(x, y, z == -1 ? zStart : zEnd, 1.0f);

					cuboidCorner = invCameraProjView * cuboidCorner;
					cuboidCorner /= cuboidCorner.w;

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

		lightCamera->SetRotation(lightDirQuat);

		lightCamera->SetOrthographicInfo (
			cuboidExtendsMin.x, cuboidExtendsMax.x,
			cuboidExtendsMin.y, cuboidExtendsMax.y,
			cuboidExtendsMin.z - LIGHT_CAMERA_OFFSET, cuboidExtendsMax.z + LIGHT_CAMERA_OFFSET
		);
	}
}

void DirectionalLightShadowMapContainerRenderSubPass::RenderScene (const Scene* scene, OrthographicCamera* lightCamera)
{
	/*
	 * Shadow map is a depth test
	*/

	GL::Enable (GL_DEPTH_TEST);
	GL::DepthMask (GL_TRUE);

	/*
	 * Doesn't really matter
	*/

	GL::Enable(GL_BLEND);
	GL::BlendFunc(GL_ONE, GL_ZERO);

	/*
	 * Enable front face culling
	*/

	GL::Enable(GL_CULL_FACE);
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

void DirectionalLightShadowMapContainerRenderSubPass::LockShader (int sceneLayers)
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
