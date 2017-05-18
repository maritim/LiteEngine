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
	_lightCamera (new OrthographicCamera ())
{
	_shaderName = "SHADOW_MAP_DIRECTIONAL_LIGHT";

	ShaderManager::Instance ()->AddShader (_shaderName,
		"Assets/Shaders/ShadowMap/deferredDirVolShadowMapLightVertex.glsl",
		"Assets/Shaders/SHadowMap/deferredDirVolShadowMapLightFragment.glsl");

	_volume = new ShadowMapDirectionalLightVolume ();
	_volume->Init ();
}

Scene* mainScene = nullptr;

void DirectionalLightShadowMapRenderer::ShadowMapRender (Scene* scene, Camera* camera)
{
	/*
	 * Render scene entities to framebuffer at Deferred Rendering Stage
	*/

	mainScene = scene;

	UpdateLightCamera (camera);
	SendLightCamera (camera);

	GL::Enable (GL_DEPTH_TEST);
	GL::Enable (GL_BLEND);
	GL::Disable (GL_CULL_FACE);

	GL::BlendFunc (GL_ONE, GL_ZERO);

	FrustumVolume* frustum = _lightCamera->GetFrustumVolume ();

	for (std::size_t i=0;i<scene->GetObjectsCount ();i++) {
		if (scene->GetObjectAt (i)->GetRenderer ()->GetStageType () != Renderer::StageType::DEFERRED_STAGE) {
			continue;
		}

		/*
		 * Culling Check
		*/

		if (scene->GetObjectAt (i)->GetCollider () == nullptr) {
			continue;
		}

		GeometricPrimitive* primitive = scene->GetObjectAt (i)->GetCollider ()->GetGeometricPrimitive ();
		if (!Intersection::Instance ()->CheckFrustumVsPrimitive (frustum, primitive)) {
			continue;
		}

		scene->GetObjectAt (i)->GetRenderer ()->Draw ();
	}
}

void DirectionalLightShadowMapRenderer::SendLightCamera (Camera* viewCamera)
{
	Pipeline::CreateProjection (_lightCamera->GetProjectionMatrix ());
	Pipeline::SendCamera (_lightCamera);
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

void DirectionalLightShadowMapRenderer::UpdateLightCamera(Camera* viewCamera)
{
	const float LIGHT_CAMERA_OFFSET = 100.0f;

	glm::vec3 lightDir = glm::normalize(_transform->GetPosition()) * -1.0f;
	glm::quat lightDirQuat = glm::toQuat(glm::lookAt(glm::vec3 (0, 0, 0), lightDir, glm::vec3(0, 1, 0)));
	glm::mat4 lightView = glm::translate (glm::mat4_cast (lightDirQuat), glm::vec3 ());

	glm::mat4 cameraView = glm::translate(glm::mat4_cast(viewCamera->GetRotation()), viewCamera->GetPosition() * -1.0f);
	glm::mat4 cameraProjection = viewCamera->GetProjectionMatrix();
	glm::mat4 invCameraProjView = glm::inverse(cameraProjection * cameraView);

	glm::vec3 cuboidExtendsMin = glm::vec3(1, 1, 1) * std::numeric_limits<float>::max ();
	glm::vec3 cuboidExtendsMax = glm::vec3(1, 1, 1) * -std::numeric_limits<float>::min ();

	for (int i = -1; i <= 1; i += 2) {
		for (int j = -1; j <= 1; j += 2) {
			for (int k = -1; k <= 1; k += 2) {
				glm::vec4 cuboidCorner = glm::vec4(i, j, k, 1.0f);

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

	_lightCamera->SetRotation(lightDirQuat);

	_lightCamera->SetOrthographicInfo (
		cuboidExtendsMin.x - LIGHT_CAMERA_OFFSET, cuboidExtendsMax.x + LIGHT_CAMERA_OFFSET,
		cuboidExtendsMin.y - LIGHT_CAMERA_OFFSET, cuboidExtendsMax.y + LIGHT_CAMERA_OFFSET,
		cuboidExtendsMin.z - LIGHT_CAMERA_OFFSET, cuboidExtendsMax.z + LIGHT_CAMERA_OFFSET
	);
}

std::vector<PipelineAttribute> DirectionalLightShadowMapRenderer::GetCustomAttributes ()
{
	std::vector<PipelineAttribute> attributes = LightShadowMapRenderer::GetCustomAttributes ();

	PipelineAttribute shadowMap;
	PipelineAttribute lightSpaceMatrix;

	shadowMap.type = PipelineAttribute::AttrType::ATTR_1I;
	lightSpaceMatrix.type = PipelineAttribute::AttrType::ATTR_MATRIX_4X4F;

	shadowMap.name = "shadowMap";
	lightSpaceMatrix.name = "lightSpaceMatrix";

	shadowMap.value.x = 4;

	glm::mat4 lightProjection = _lightCamera->GetProjectionMatrix ();
	glm::mat4 lightView = glm::translate (glm::mat4_cast (_lightCamera->GetRotation ()), _lightCamera->GetPosition () * -1.0f);

	lightSpaceMatrix.matrix = lightProjection * lightView;

	attributes.push_back (shadowMap);
	attributes.push_back (lightSpaceMatrix);

	return attributes;
}