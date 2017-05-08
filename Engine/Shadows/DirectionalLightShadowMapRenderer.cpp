#include "DirectionalLightShadowMapRenderer.h"

#include "ShadowMapDirectionalLightVolume.h"

#include "Managers/ShaderManager.h"

#include "Core/Intersections/Intersection.h"

#include "Renderer/Pipeline.h"

#include "Debug/Logger/Logger.h"

DirectionalLightShadowMapRenderer::DirectionalLightShadowMapRenderer (Light* light) :
	LightShadowMapRenderer (light)
{
	_shaderName = "SHADOW_MAP_DIRECTIONAL_LIGHT";

	ShaderManager::Instance ()->AddShader (_shaderName,
		//"Assets/Shaders/ShadowMap/deferredDirVolShadowMapLightVertex.glsl",
		//"Assets/Shaders/SHadowMap/deferredDirVolShadowMapLightFragment.glsl");
		"Assets/Shaders/deferredDirVolLightVertex.glsl",
		"Assets/Shaders/deferredDirVolLightFragment.glsl");

	_volume = new ShadowMapDirectionalLightVolume ();
}

void DirectionalLightShadowMapRenderer::ShadowMapRender (Scene* scene, Camera* camera)
{
	/*
	 * Render scene entities to framebuffer at Deferred Rendering Stage
	*/

	SendLightCamera (camera);

	FrustumVolume* frustum = camera->GetFrustumVolume ();

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
	OrthographicCamera* lightCamera = CreateLightCamera (viewCamera);

	Pipeline::CreateProjection (lightCamera->GetProjectionMatrix ());
	Pipeline::SendCamera (lightCamera);

	delete lightCamera;
}

OrthographicCamera* DirectionalLightShadowMapRenderer::CreateLightCamera (Camera* viewCamera)
{
	OrthographicCamera* orthoCamera = new OrthographicCamera ();

	orthoCamera->SetPosition (glm::vec3 (100, 100, 100));
	orthoCamera->SetRotation (_transform->GetPosition ());

	orthoCamera->SetOrthographicSize(100);

	return orthoCamera;
}

std::vector<PipelineAttribute> DirectionalLightShadowMapRenderer::GetCustomAttributes ()
{
	std::vector<PipelineAttribute> attributes = LightShadowMapRenderer::GetCustomAttributes ();

	return attributes;
}