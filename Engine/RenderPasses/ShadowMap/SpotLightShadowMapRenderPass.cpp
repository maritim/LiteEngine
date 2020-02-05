#include "SpotLightShadowMapRenderPass.h"

#include "Core/Intersections/Intersection.h"

#include "Renderer/Pipeline.h"

#include "Wrappers/OpenGL/GL.h"

#include "Core/Console/Console.h"

#include "SceneNodes/SceneLayer.h"

SpotLightShadowMapRenderPass::SpotLightShadowMapRenderPass () :
	_volume (new PerspectiveShadowMapVolume ())
{

}

SpotLightShadowMapRenderPass::~SpotLightShadowMapRenderPass ()
{
	delete _volume;
}

RenderVolumeCollection* SpotLightShadowMapRenderPass::Execute (const RenderScene* renderScene, const Camera* camera,
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
	 * Update light camera
	*/

	UpdateLightCamera (renderLightObject);

	/*
	 * Draw shadow map
	*/

	ShadowMapPass (renderScene, camera);

	/*
	 * End drawing
	*/

	EndShadowMapPass ();

	return rvc->Insert ("ShadowMapSpotLightVolume", _volume);
}

void SpotLightShadowMapRenderPass::Clear ()
{
	/*
	 * Clear shadow map volume
	*/

	_volume->Clear ();
}

bool SpotLightShadowMapRenderPass::IsAvailable (const RenderLightObject* renderLightObject) const
{
	/*
	 * Execute shadow map sub pass only if light is casting shadows
	*/

	return renderLightObject->IsCastingShadows ();
}

void SpotLightShadowMapRenderPass::ShadowMapPass (const RenderScene* renderScene, const Camera* camera)
{
	/*
	 * Change resolution on viewport as shadow map size
	*/

	glm::ivec2 resolution = _volume->GetSize ();

	GL::Viewport (0, 0, resolution.x, resolution.y);

	/*
	 * Bind shadow map for writing
	*/

	_volume->BindForWriting ();

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

	PerspectiveCamera* lightCamera = _volume->GetLightCamera ();

	/*
	 * Send light camera
	*/

	Pipeline::CreateProjection (lightCamera->GetProjectionMatrix ());
	Pipeline::SendCamera (lightCamera);

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

		Pipeline::SendCustomAttributes (nullptr, GetCustomAttributes ());

		/*
		 * Render object on shadow map
		*/

		renderObject->DrawGeometry ();
	}
}

void SpotLightShadowMapRenderPass::EndShadowMapPass ()
{
	/*
	 * End shadow map pass
	*/

	Pipeline::UnlockShader ();
}

std::vector<PipelineAttribute> SpotLightShadowMapRenderPass::GetCustomAttributes () const
{
	std::vector<PipelineAttribute> attributes;

	/*
	 * Do nothing
	*/

	return attributes;
}

void SpotLightShadowMapRenderPass::UpdateLightCamera (const RenderLightObject* renderLightObject)
{
	auto renderSpotLightObject = dynamic_cast<const RenderSpotLightObject*> (renderLightObject);

	Transform* lightTransform = renderSpotLightObject->GetTransform ();

	PerspectiveCamera* lightCamera = _volume->GetLightCamera ();

	lightCamera->SetPosition (lightTransform->GetPosition ());
	lightCamera->SetRotation (glm::conjugate (lightTransform->GetRotation ()));

	lightCamera->SetZFar (renderSpotLightObject->GetLightRange ());
	lightCamera->SetFieldOfView (renderSpotLightObject->GetLightSpotOuterCutoff () * 2);
	lightCamera->SetAspect (1.0f);
}

void SpotLightShadowMapRenderPass::UpdateShadowMapVolume (const RenderLightObject* renderLightObject)
{
	RenderLightObject::Shadow shadow = renderLightObject->GetShadow ();

	if (_volume->GetSize () != shadow.resolution) {

		/*
		 * Clear shadow map volume
		*/

		_volume->Clear ();

		/*
		 * Initialize shadow map volume
		*/

		InitShadowMapVolume (renderLightObject);
	}

	_volume->SetShadowBias (shadow.bias);
}

void SpotLightShadowMapRenderPass::InitShadowMapVolume (const RenderLightObject* renderLightObject)
{
	/*
	 * Initialize shadow map volume
	*/

	RenderLightObject::Shadow shadow = renderLightObject->GetShadow ();

	if (!_volume->Init (new PerspectiveCamera (), shadow.resolution)) {
		Console::LogError (std::string () + "Shadow map cannot be initialized!" +
			" It is not possible to continue the process. End now!");
		exit (SHADOW_MAP_FBO_NOT_INIT);
	}
}