#include "DirectionalLightShadowMapRenderer.h"

#include "ShadowMapDirectionalLightVolume.h"

#include "Managers/ShaderManager.h"

#include "Core/Intersections/Intersection.h"

#include "Debug/Logger/Logger.h"

DirectionalLightShadowMapRenderer::DirectionalLightShadowMapRenderer (Light* light) :
	LightShadowMapRenderer (light)
{
	_shaderName = "SHADOW_MAP_DIRECTIONAL_LIGHT";

	ShaderManager::Instance ()->AddShader (_shaderName,
		"Assets/Shaders/deferredDirVolLightVertex.glsl",
		"Assets/Shaders/deferredDirVolLightFragment.glsl");

	_volume = new ShadowMapDirectionalLightVolume ();
}

void DirectionalLightShadowMapRenderer::ShadowMapRender (Scene* scene, Camera* camera)
{
	/*
	 * Render scene entities to framebuffer at Deferred Rendering Stage
	*/

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

	DEBUG_LOG ("ShadowMap");
	
	// 2. then render scene as normal with shadow mapping (using depth map)
	// glBindTexture(GL_TEXTURE_2D, depthMap);
	// RenderScene();
}

std::vector<PipelineAttribute> DirectionalLightShadowMapRenderer::GetCustomAttributes ()
{
	std::vector<PipelineAttribute> attributes = LightShadowMapRenderer::GetCustomAttributes ();

	return attributes;
}