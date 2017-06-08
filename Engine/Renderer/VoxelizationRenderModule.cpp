#include "VoxelizationRenderModule.h"

#include <vector>
#include <algorithm>

#include "Core/Math/glm/gtx/quaternion.hpp"
#include "Core/Math/glm/gtc/matrix_transform.hpp"
#include "Core/Math/glm/gtc/quaternion.hpp"

#include "Renderer.h"

#include "Lighting/LightsManager.h"
#include "Cameras/OrthographicCamera.h"

#include "Pipeline.h"

#include "Core/Intersections/Intersection.h"

#include "Wrappers/OpenGL/GL.h"

#include "Systems/Window/Window.h"

#include "Debug/Profiler/Profiler.h"

#include "Core/Console/Console.h"

#include "Settings/GeneralSettings.h"

#define VOLUME_DIMENSTIONS 512

VoxelizationRenderModule::VoxelizationRenderModule () :
	_voxelVolume (new VoxelVolume ()),
	_shadowMapVolume (new ShadowMapDirectionalLightVolume ())
{
	_voxelVolume->Init (VOLUME_DIMENSTIONS);

	if (!_shadowMapVolume->Init (1)) {
		Console::LogError (std::string () + "Shadow map cannot be initialized!" +
			" It is not possible to continue the process. End now!");
		exit (SHADOW_MAP_FBO_NOT_INIT);
	}
}

VoxelizationRenderModule::~VoxelizationRenderModule ()
{
	delete _voxelVolume;
}

void VoxelizationRenderModule::RenderScene (Scene* scene, Camera* camera)
{
	/*
	 * Send Camera to Pipeline
	*/

	UpdateCamera (camera);

	/*
	 * Voxelize the scene
	*/

	if (GeneralSettings::Instance ()->GetIntValue ("ContinousVoxelizationPass")) {
		VoxelizePass (scene, camera);
	}

	/*
	 * Voxel Ray Trace Pass
	*/

	VoxelRayTracePass ();
}


void VoxelizationRenderModule::VoxelizePass (Scene* scene, Camera* camera)
{
	PROFILER_LOGGER ("Voxelization Pass");

	/*
	 * Geometry voxelization pass
	*/

	VoxelPass (scene);

	/*
	 * Radiance injection pass on voxel volume
	*/

	if (GeneralSettings::Instance ()->GetIntValue ("RadianceInjection")) { 
		RadianceInjectionPass (scene, camera);
	}

	/*
	 * Mipmaping pass for voxel
	*/

	MipmappingPass ();
}

void VoxelizationRenderModule::VoxelPass (Scene* scene)
{
	/*
	 * Voxelization start
	*/

	StartVoxelization ();

	/*
	 * Voxelization: voxelize geomtry
	*/

	GeometryVoxelizationPass (scene);

	/*
	 * Clear opengl state after voxelization
	*/

	EndVoxelization ();
}

void VoxelizationRenderModule::MipmappingPass ()
{
	/*
	* Start mipmapping pass
	*/

	StartVoxelMipmaping ();

	/*
	* Mipmapping pass
	*/

	_voxelVolume->GenerateMipmaps ();

	/*
	* End mipmapping pass
	*/

	EndVoxelMipmaping ();
}

void VoxelizationRenderModule::StartVoxelization()
{
	_voxelVolume->ClearVoxels();

	_voxelVolume->StartVoxelizationPass ();
}

void VoxelizationRenderModule::GeometryVoxelizationPass(Scene* scene)
{
	/*
	* Update voxel volume based on scene bounding box
	*/

	UpdateVoxelVolumeBoundingBox (scene);

	/*
	* Bind voxel volume to geometry render pass
	*/

	_voxelVolume->BindForVoxelizationPass ();

	/*
	* Render geometry
	*/

	for (SceneObject* sceneObject : *scene) {
		if (sceneObject->GetRenderer ()->GetStageType () != Renderer::StageType::DEFERRED_STAGE) {
			continue;
		}

		sceneObject->GetRenderer ()->Draw ();
	}
}

void VoxelizationRenderModule::EndVoxelization()
{
	_voxelVolume->EndVoxelizationPass ();
}

void VoxelizationRenderModule::StartVoxelMipmaping ()
{
	_voxelVolume->StartVoxelMipmapPass ();
}

void VoxelizationRenderModule::EndVoxelMipmaping ()
{
	_voxelVolume->EndVoxelMipmapPass ();
}

void VoxelizationRenderModule::UpdateVoxelVolumeBoundingBox(Scene* scene)
{
	AABBVolume* boundingBox = scene->GetBoundingBox();
	AABBVolume::AABBVolumeInformation* volume = boundingBox->GetVolumeInformation();

	glm::vec3 minVertex = volume->minVertex;
	glm::vec3 maxVertex = volume->maxVertex;

	_voxelVolume->UpdateBoundingBox (minVertex, maxVertex);
}

void VoxelizationRenderModule::RadianceInjectionPass (Scene* scene, Camera* camera)
{
	/*
	* Shadow map rendering pass
	*/

	ShadowMapPass (scene, camera);

	/*
	* Inject shadow map into voxel volume
	*/

	RadianceShadowMapInjectionPass ();
}

void VoxelizationRenderModule::ShadowMapPass (Scene* scene, Camera* camera)
{
	/*
	* Start shadow map drawing process
	*/

	StartShadowMapPass ();

	/*
	* Calculate light camera for shadow map
	*/

	lightCamera = GetLightCamera (scene, camera);

	/*
	* Render geometry on shadow map
	*/

	ShadowMapGeometryPass (scene, lightCamera);

	/*
	* End shadow map drawing process
	*/

	EndShadowMapPass ();
}

void VoxelizationRenderModule::RadianceShadowMapInjectionPass ()
{
	/*
	* Start radiance injecting pass
	*/

	StartRadianceInjectionPass ();

	/*
	* Radiance Injecting pass
	*/

	RadianceInjectPass ();

	/*
	* End radiance injecting pass
	*/

	EndRadianceInjectionPass ();
}

void VoxelizationRenderModule::StartShadowMapPass ()
{
	_shadowMapVolume->BindForShadowMapCatch (0);
}

void VoxelizationRenderModule::ShadowMapGeometryPass (Scene* scene, Camera* lightCamera)
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

		_shadowMapVolume->LockShader (sceneObject->GetLayers ());

		/*
		* Render object on shadow map
		*/

		sceneObject->GetRenderer ()->Draw ();
	}
}

void VoxelizationRenderModule::EndShadowMapPass ()
{
	_shadowMapVolume->EndDrawing ();
}

void VoxelizationRenderModule::StartRadianceInjectionPass ()
{
	_voxelVolume->StartVoxelRadianceInjectionPass ();
}

void VoxelizationRenderModule::RadianceInjectPass ()
{
	_shadowMapVolume->BindForReading ();

	_voxelVolume->BindForRadianceInjectionPass ();

	UpdateShadowMapPipelineAttributes ();

	_voxelVolume->InjectRadiance ();
}

void VoxelizationRenderModule::UpdateShadowMapPipelineAttributes ()
{
	std::vector<PipelineAttribute> attributes;

	PipelineAttribute shadowMap;
	PipelineAttribute lightSpaceMatrix;

	shadowMap.type = PipelineAttribute::AttrType::ATTR_1I;
	lightSpaceMatrix.type = PipelineAttribute::AttrType::ATTR_MATRIX_4X4F;

	shadowMap.name = "shadowMap";
	lightSpaceMatrix.name = "lightSpaceMatrix";

	shadowMap.value.x = 4;

	glm::mat4 lightProjection = lightCamera->GetProjectionMatrix ();
	glm::mat4 lightView = glm::translate (glm::mat4_cast (lightCamera->GetRotation ()), lightCamera->GetPosition () * -1.0f);

	lightSpaceMatrix.matrix = lightProjection * lightView;

	attributes.push_back (shadowMap);
	attributes.push_back (lightSpaceMatrix);

	Pipeline::SendCustomAttributes ("VOXEL_RADIANCE_INJECTION_COMPUTE_SHADER", attributes);
}

void VoxelizationRenderModule::EndRadianceInjectionPass ()
{
	_voxelVolume->EndVoxelRadianceInjectionPass ();
}

Camera* VoxelizationRenderModule::GetLightCamera (Scene* scene, Camera* viewCamera)
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

	AABBVolume* sceneBoundingBox = scene->GetBoundingBox ();
	AABBVolume::AABBVolumeInformation* boundingBoxInfo = sceneBoundingBox->GetVolumeInformation ();

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

void VoxelizationRenderModule::VoxelRayTracePass ()
{
	PROFILER_LOGGER ("Ray Trace Pass");

	/*
	 * Start Ray Trace Pass
	*/

	StartRayTrace ();

	/*
	 * Ray Trace Rendering Pass
	*/

	VoxelRenderingRayTracePass ();
}

void VoxelizationRenderModule::StartRayTrace()
{
	_voxelVolume->StartRayTracePass ();
}

void VoxelizationRenderModule::VoxelRenderingRayTracePass()
{
	_voxelVolume->BindForRayTracePass ();

	/*
	 * Clear framebuffer
	*/

	GL::Clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	/*
	 * Render fullscreen quad.	
	*/

	GL::DrawArrays (GL_POINTS, 0, 1);
}

/*
 * TODO: Move this part somewhere else because it belongs to another
 * abstraction layer. This class only work with objects rendering, not
 * pipeline's job
*/
	
void VoxelizationRenderModule::UpdateCamera (Camera* camera)
{
	// Create Projection
	Pipeline::CreateProjection (camera);

	// Create View Matrix
	Pipeline::SendCamera (camera);	
}
