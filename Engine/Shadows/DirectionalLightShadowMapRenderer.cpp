#include "DirectionalLightShadowMapRenderer.h"

#include <algorithm>
#include <limits>

#include "Core/Math/glm/gtx/quaternion.hpp"
#include "Core/Math/glm/gtc/matrix_transform.hpp"
#include "Core/Math/glm/gtc/quaternion.hpp"

#include "ShadowMapDirectionalLightVolume.h"

#include "Managers/ShaderManager.h"

#include "Core/Intersections/Intersection.h"

#include "Wrappers/OpenGL/GL.h"
#include "Renderer/Pipeline.h"

DirectionalLightShadowMapRenderer::DirectionalLightShadowMapRenderer (Light* light) :
	LightShadowMapRenderer (light),
	_lightCameras (new OrthographicCamera* [CASCADED_SHADOW_MAP_LEVELS]),
	_shadowMapZEnd (new float [CASCADED_SHADOW_MAP_LEVELS + 1])
{
	_shaderName = "SHADOW_MAP_DIRECTIONAL_LIGHT";

	ShaderManager::Instance ()->AddShader (_shaderName,
		"Assets/Shaders/ShadowMap/deferredDirVolShadowMapLightVertex.glsl",
		"Assets/Shaders/ShadowMap/deferredDirVolShadowMapLightFragment.glsl");

	_volume = new ShadowMapDirectionalLightVolume ();

	for (std::size_t index = 0; index < CASCADED_SHADOW_MAP_LEVELS; index++) {
		_lightCameras [index] = new OrthographicCamera ();
	}
}

void DirectionalLightShadowMapRenderer::ShadowMapRender (Scene* scene, Camera* camera)
{
	UpdateCascadeLevelsLimits (camera);
	UpdateLightCameras (camera);

	for (std::size_t index = 0; index < CASCADED_SHADOW_MAP_LEVELS; index++) {
		_volume->BindForShadowMapCatch (index);

		OrthographicCamera* lightCamera = _lightCameras [index];

		SendLightCamera (lightCamera);
		RenderScene (scene, lightCamera);
	}
}

void DirectionalLightShadowMapRenderer::UpdateCascadeLevelsLimits (Camera* camera)
{
	/*
	 * TODO: Calculate this at run time
	*/

	_shadowMapZEnd [0] = 0.92f;
	_shadowMapZEnd [1] = 0.98f;
	_shadowMapZEnd [2] = 0.995f;
	_shadowMapZEnd [3] = 1.0f;
}

void DirectionalLightShadowMapRenderer::SendLightCamera (Camera* lightCamera)
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

void DirectionalLightShadowMapRenderer::UpdateLightCameras (Camera* viewCamera)
{
	const float LIGHT_CAMERA_OFFSET = 100.0f;

	glm::vec3 lightDir = glm::normalize(_transform->GetPosition()) * -1.0f;
	glm::quat lightDirQuat = glm::toQuat(glm::lookAt(glm::vec3 (0), lightDir, glm::vec3(0, 1, 0)));
	glm::mat4 lightView = glm::translate (glm::mat4_cast (lightDirQuat), glm::vec3 (0));

	glm::mat4 cameraView = glm::translate(glm::mat4_cast(viewCamera->GetRotation()), viewCamera->GetPosition() * -1.0f);
	glm::mat4 cameraProjection = viewCamera->GetProjectionMatrix();
	glm::mat4 invCameraProjView = glm::inverse(cameraProjection * cameraView);

	for (std::size_t index = 0; index < CASCADED_SHADOW_MAP_LEVELS; index++) {

		glm::vec3 cuboidExtendsMin = glm::vec3(std::numeric_limits<float>::max());
		glm::vec3 cuboidExtendsMax = glm::vec3(-std::numeric_limits<float>::min());

		float zStart = index == 0 ? - 1 : _shadowMapZEnd [index - 1];
		float zEnd = _shadowMapZEnd [index];

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

		_lightCameras [index]->SetRotation(lightDirQuat);

		_lightCameras [index]->SetOrthographicInfo (
			cuboidExtendsMin.x, cuboidExtendsMax.x,
			cuboidExtendsMin.y, cuboidExtendsMax.y,
			cuboidExtendsMin.z - LIGHT_CAMERA_OFFSET, cuboidExtendsMax.z + LIGHT_CAMERA_OFFSET
		);
	}
}

void DirectionalLightShadowMapRenderer::RenderScene (Scene* scene, OrthographicCamera* lightCamera)
{
	/*
	 * Shadow map is a depth test
	*/

	GL::Enable (GL_DEPTH_TEST);

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

	for (std::size_t index=0; index<scene->GetObjectsCount (); index++) {
		if (scene->GetObjectAt (index)->GetRenderer ()->GetStageType () != Renderer::StageType::DEFERRED_STAGE) {
			continue;
		}

		/*
		 * Culling Check
		*/

		if (scene->GetObjectAt (index)->GetCollider () == nullptr) {
			continue;
		}

		GeometricPrimitive* primitive = scene->GetObjectAt (index)->GetCollider ()->GetGeometricPrimitive ();
		if (!Intersection::Instance ()->CheckFrustumVsPrimitive (frustum, primitive)) {
			continue;
		}

		/*
		 * Lock shader based on scene object layer
		*/

		_volume->LockShader (scene->GetObjectAt (index)->GetLayers ());

		/*
		 * Render object on shadow map
		*/

		scene->GetObjectAt (index)->GetRenderer ()->Draw ();
	}
}

std::vector<PipelineAttribute> DirectionalLightShadowMapRenderer::GetCustomAttributes ()
{
	std::vector<PipelineAttribute> attributes = LightShadowMapRenderer::GetCustomAttributes ();

	for (std::size_t index = 0; index<CASCADED_SHADOW_MAP_LEVELS; index++) {

		PipelineAttribute shadowMap;
		PipelineAttribute lightSpaceMatrix;
		PipelineAttribute clipZLevel;

		shadowMap.type = PipelineAttribute::AttrType::ATTR_1I;
		lightSpaceMatrix.type = PipelineAttribute::AttrType::ATTR_MATRIX_4X4F;
		clipZLevel.type = PipelineAttribute::AttrType::ATTR_1F;

		shadowMap.name = "shadowMaps[" + std::to_string (index) + "]";
		lightSpaceMatrix.name = "lightSpaceMatrices[" + std::to_string (index) + "]";
		clipZLevel.name = "clipZLevels[" + std::to_string (index) + "]";

		shadowMap.value.x = 4.0f + index;

		glm::mat4 lightProjection = _lightCameras [index]->GetProjectionMatrix ();
		glm::mat4 lightView = glm::translate (glm::mat4_cast(_lightCameras [index]->GetRotation ()), _lightCameras [index]->GetPosition () * -1.0f);

		lightSpaceMatrix.matrix = lightProjection * lightView;

		clipZLevel.value.x = _shadowMapZEnd [index];

		attributes.push_back (shadowMap);
		attributes.push_back (lightSpaceMatrix);
		attributes.push_back (clipZLevel);
	}

	return attributes;
}