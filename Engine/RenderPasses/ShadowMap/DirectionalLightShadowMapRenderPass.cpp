#include "DirectionalLightShadowMapRenderPass.h"

#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <algorithm>
#include <cmath>

#include "Managers/ShaderManager.h"

#include "Core/Intersections/Intersection.h"

#include "Renderer/Pipeline.h"

#include "Core/Console/Console.h"

#include "SceneNodes/SceneLayer.h"

#include "Debug/Statistics/StatisticsManager.h"
#include "Debug/Statistics/LightStatisticsObject.h"

DirectionalLightShadowMapRenderPass::DirectionalLightShadowMapRenderPass () :
	_staticShaderName ("STATIC_SHADOW_MAP"),
	_animationShaderName ("ANIMATION_SHADOW_MAP"),
	_volume (new CascadedShadowMapDirectionalLightVolume ())
{

}

DirectionalLightShadowMapRenderPass::~DirectionalLightShadowMapRenderPass ()
{
	delete _volume;
}

void DirectionalLightShadowMapRenderPass::Init (const RenderSettings& settings)
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
}

RenderVolumeCollection* DirectionalLightShadowMapRenderPass::Execute (const RenderScene* renderScene, const Camera* camera,
	const RenderSettings& settings, RenderVolumeCollection* rvc)
{
	/*
	 * Get volumetric light from render volume collection
	*/

	RenderLightObject* renderLightObject = GetRenderLightObject (rvc);

	/*
	 * Update shadow map volume
	*/

	UpdateShadowMapVolume (renderLightObject);

	/*
	 * Draw shadow map
	*/

	ShadowMapPass (renderScene, camera, renderLightObject);

	/*
	 * End drawing
	*/

	EndShadowMapPass ();

	return rvc->Insert ("ShadowMapDirectionalLightVolume", _volume);
}

void DirectionalLightShadowMapRenderPass::Clear ()
{
	/*
	 * Clear shadow map volume
	*/

	_volume->Clear ();
}

bool DirectionalLightShadowMapRenderPass::IsAvailable (const RenderLightObject* renderLightObject) const
{
	/*
	 * Execute shadow map sub pass only if light is casting shadows
	*/

	return renderLightObject->IsCastingShadows ();
}

void DirectionalLightShadowMapRenderPass::ShadowMapPass (const RenderScene* renderScene, const Camera* camera, const RenderLightObject* renderLightObject)
{
	UpdateCascadeLevelsLimits (camera, renderLightObject);
	UpdateLightCameras (camera, renderLightObject);

	RenderLightObject::Shadow shadow = renderLightObject->GetShadow ();

	StatisticsObject* stat = StatisticsManager::Instance ()->GetStatisticsObject ("LightStatisticsObject");
	LightStatisticsObject* lightStatisticsObject = nullptr;

	if (stat == nullptr) {
		stat = new LightStatisticsObject ();
		StatisticsManager::Instance ()->SetStatisticsObject ("LightStatisticsObject", stat);
	}

	lightStatisticsObject = dynamic_cast<LightStatisticsObject*> (stat);

	lightStatisticsObject->dirLightShadowMaps.clear ();

	for (std::size_t index = 0; index < shadow.cascadesCount; index++) {
		_volume->BindForShadowMapCatch (index);

		OrthographicCamera* lightCamera = (OrthographicCamera*) _volume->GetLightCamera (index);

		SendLightCamera (lightCamera);
		Render (renderScene, lightCamera);

		lightStatisticsObject->dirLightShadowMaps.push_back (_volume->GetShadowMapVolume (index));
	}
}

void DirectionalLightShadowMapRenderPass::EndShadowMapPass ()
{
	/*
	 * End shadow map pass
	*/

	Pipeline::UnlockShader ();
}

void DirectionalLightShadowMapRenderPass::UpdateCascadeLevelsLimits (const Camera* camera, const RenderLightObject* renderLightObject)
{
	/*
	 * Practical split scheme
	 * Thanks to: https://developer.nvidia.com/gpugems/GPUGems3/gpugems3_ch10.html
	*/

	RenderLightObject::Shadow shadow = renderLightObject->GetShadow ();

	float l = 0.5f;

	for (std::size_t cascadeIndex = 0; cascadeIndex < shadow.cascadesCount; cascadeIndex ++) {

		float zNear = camera->GetZNear ();
		float zFar = camera->GetZFar ();

		/*
		 * Compute distance limit
		*/

		float dLimit1 = std::pow (zNear * (zFar / zNear), (float) (cascadeIndex + 1) / shadow.cascadesCount);
		float dLimit2 = zNear + (zFar - zNear) * ((float) (cascadeIndex + 1) / shadow.cascadesCount);

		float dLimit = l * dLimit1 + (1.0f - l) * dLimit2;

		/*
		 * Compute projection z limit
		*/

		glm::mat4 projMatrix = camera->GetProjectionMatrix ();

		glm::vec4 pLimit = projMatrix * glm::vec4 (0, 0, -dLimit, 1);

		float zLimit = pLimit.z / pLimit.w;

		/*
		 * Attach z limit to shadow map volume
		*/

		_volume->SetCameraLimit (cascadeIndex, zLimit);
	}
}

void DirectionalLightShadowMapRenderPass::SendLightCamera (Camera* lightCamera)
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

void DirectionalLightShadowMapRenderPass::UpdateLightCameras (const Camera* viewCamera, const RenderLightObject* renderLightObject)
{
	const float LIGHT_CAMERA_OFFSET = 100.0f;

	glm::quat lightRotation = glm::conjugate (renderLightObject->GetTransform ()->GetRotation ());

	glm::mat4 cameraView = glm::translate(glm::mat4_cast(viewCamera->GetRotation()), viewCamera->GetPosition() * -1.0f);
	glm::mat4 cameraProjection = viewCamera->GetProjectionMatrix();
	glm::mat4 invCameraProjView = glm::inverse(cameraProjection * cameraView);

	RenderLightObject::Shadow shadow = renderLightObject->GetShadow ();

	for (std::size_t index = 0; index < shadow.cascadesCount; index++) {

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

					cuboidCorner = glm::vec4 (lightRotation * glm::vec3 (cuboidCorner), 0.0f);

					cuboidExtendsMin.x = std::min(cuboidExtendsMin.x, cuboidCorner.x);
					cuboidExtendsMin.y = std::min(cuboidExtendsMin.y, cuboidCorner.y);
					cuboidExtendsMin.z = std::min(cuboidExtendsMin.z, cuboidCorner.z);

					cuboidExtendsMax.x = std::max(cuboidExtendsMax.x, cuboidCorner.x);
					cuboidExtendsMax.y = std::max(cuboidExtendsMax.y, cuboidCorner.y);
					cuboidExtendsMax.z = std::max(cuboidExtendsMax.z, cuboidCorner.z);
				}
			}
		}

		lightCamera->SetRotation(lightRotation);

		lightCamera->SetOrthographicInfo (
			cuboidExtendsMin.x, cuboidExtendsMax.x,
			cuboidExtendsMin.y, cuboidExtendsMax.y,
			cuboidExtendsMin.z - LIGHT_CAMERA_OFFSET, cuboidExtendsMax.z + LIGHT_CAMERA_OFFSET
		);
	}
}

void DirectionalLightShadowMapRenderPass::Render (const RenderScene* renderScene, OrthographicCamera* lightCamera)
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
		 * Lock shader based on scene object layer
		*/

		LockShader (renderObject->GetSceneLayers ());

		/*
		 * Send custom attributes
		*/

		Pipeline::SendCustomAttributes ("", GetCustomAttributes ());

		/*
		 * Render object on shadow map
		*/

		renderObject->DrawGeometry ();
	}
}

void DirectionalLightShadowMapRenderPass::LockShader (int sceneLayers)
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

std::vector<PipelineAttribute> DirectionalLightShadowMapRenderPass::GetCustomAttributes () const
{
	std::vector<PipelineAttribute> attributes;

	/*
	 * Do nothing
	*/

	return attributes;
}

void DirectionalLightShadowMapRenderPass::UpdateShadowMapVolume (const RenderLightObject* renderLightObject)
{
	RenderLightObject::Shadow shadow = renderLightObject->GetShadow ();

	if (_volume->GetCascadesCount () != shadow.cascadesCount ||
		_volume->GetShadowMapVolume (0)->GetSize () != shadow.resolution) {

		/*
		 * Clear shadow map volume
		*/

		_volume->Clear ();

		/*
		 * Initialize shadow map volume
		*/

		InitShadowMapVolume (renderLightObject);
	}
}

void DirectionalLightShadowMapRenderPass::InitShadowMapVolume (const RenderLightObject* renderLightObject)
{
	/*
	 * Initialize shadow map volume
	*/

	RenderLightObject::Shadow shadow = renderLightObject->GetShadow ();

	if (!_volume->Init (shadow.cascadesCount, shadow.resolution)) {
		Console::LogError (std::string () + "Shadow map cannot be initialized!" +
			" It is not possible to continue the process. End now!");
		exit (SHADOW_MAP_FBO_NOT_INIT);
	}

	for (std::size_t index = 0; index < shadow.cascadesCount; index ++) {
		_volume->SetLightCamera (index, new OrthographicCamera ());
	}
}