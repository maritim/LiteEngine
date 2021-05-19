#include "HybridRSMDirectionalLightViewAccumulationRenderPass.h"

#include "Core/Intersections/Intersection.h"

#include "Renderer/Pipeline.h"

#include "Wrappers/OpenGL/GL.h"

HybridRSMDirectionalLightViewAccumulationRenderPass::HybridRSMDirectionalLightViewAccumulationRenderPass () :
	_haltonGenerator (2, 3)
{

}

void HybridRSMDirectionalLightViewAccumulationRenderPass::ShadowMapGeometryPass (const RenderScene* renderScene, const Camera* lightCamera,
	const RenderSettings& settings, const RenderLightObject* renderLightObject)
{
	/*
	* Send light camera
	*/

	UpdateCamera (lightCamera, settings);

	/*
	 * Set viewport
	*/

	RenderLightObject::Shadow shadow = renderLightObject->GetShadow ();

	GL::Viewport (0, 0, shadow.resolution.x, shadow.resolution.y);

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
	 * Enable back face culling
	*/

	GL::Enable (GL_CULL_FACE);
	GL::CullFace (GL_BACK);

	/*
	* Light camera
	*/

	auto frustum = lightCamera->GetFrustumVolume ();

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

		auto& boundingBox = renderObject->GetBoundingBox ();
		if (!Intersection::Instance ()->CheckFrustumVsAABB (frustum, boundingBox)) {
			continue;
		}

		/*
		* Lock shader based on scene object layers
		*/

		LockShader (renderObject->GetSceneLayers ());

		/*
		 * Send custom attributes
		*/

		Pipeline::SendCustomAttributes (nullptr, GetCustomAttributes (renderLightObject));

		/*
		* Render object on shadow map
		*/

		renderObject->Draw ();
	}
}

void HybridRSMDirectionalLightViewAccumulationRenderPass::UpdateCamera (const Camera* lightCamera,
	const RenderSettings& settings)
{
	glm::mat4 projectionMatrix = lightCamera->GetProjectionMatrix ();

	if (settings.hgi_temporal_filter_enabled == true) {
		glm::vec2 jitter = _haltonGenerator.Next () - 0.5f;

		jitter /= glm::vec2 (settings.viewport.width, settings.viewport.height);

		glm::mat4 jitteringMatrix = glm::translate (glm::mat4 (1.0f), glm::vec3 (jitter, 0.0f));
		projectionMatrix = jitteringMatrix * projectionMatrix;
	}

	Pipeline::CreateProjection (projectionMatrix);
	Pipeline::SendCamera (lightCamera);
}
