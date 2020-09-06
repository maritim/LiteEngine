#include "SpotLightShadowMapRenderPass.h"

#include "Core/Intersections/Intersection.h"

#include "Renderer/Pipeline.h"

#include "Wrappers/OpenGL/GL.h"

#include "Core/Console/Console.h"

#include "SceneNodes/SceneLayer.h"

SpotLightShadowMapRenderPass::SpotLightShadowMapRenderPass () :
	_volume (nullptr)
{

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

	return rvc->Insert ("ShadowMapSpotLightVolume", _volume, false);
}

void SpotLightShadowMapRenderPass::Clear ()
{
	/*
	 * Clear shadow map volume
	*/

	delete _volume;
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

	auto resolution = _volume->GetFramebuffer ()->GetDepthTexture ()->GetSize ();

	GL::Viewport (0, 0, resolution.width, resolution.height);

	/*
	 * Bind shadow map for writing
	*/

	_volume->GetFramebufferView ()->Activate ();

	GL::Clear (GL_DEPTH_BUFFER_BIT);

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

	const Transform* lightTransform = renderSpotLightObject->GetTransform ();

	PerspectiveCamera* lightCamera = _volume->GetLightCamera ();

	lightCamera->SetPosition (lightTransform->GetPosition ());
	lightCamera->SetRotation (glm::conjugate (lightTransform->GetRotation ()));

	lightCamera->SetZNear (0.05f);
	lightCamera->SetZFar (renderSpotLightObject->GetLightRange ());
	lightCamera->SetFieldOfViewAngle (renderSpotLightObject->GetLightSpotOuterCutoff () * 2);
	lightCamera->SetAspect (1.0f);

	_volume->SetLightCamera (lightCamera);
}

void SpotLightShadowMapRenderPass::UpdateShadowMapVolume (const RenderLightObject* renderLightObject)
{
	RenderLightObject::Shadow shadow = renderLightObject->GetShadow ();

	if (_volume == nullptr ||
		_volume->GetFramebuffer ()->GetDepthTexture ()->GetSize ().width != (std::size_t) shadow.resolution.x ||
		_volume->GetFramebuffer ()->GetDepthTexture ()->GetSize ().height != (std::size_t) shadow.resolution.y) {

		/*
		 * Clear shadow map volume
		*/

		delete _volume;

		/*
		 * Initialize shadow map volume
		*/

		InitShadowMapVolume (renderLightObject);
	}
}

void SpotLightShadowMapRenderPass::InitShadowMapVolume (const RenderLightObject* renderLightObject)
{
	/*
	 * Create perspective shadow map volume
	*/

	Resource<Texture> texture = Resource<Texture> (new Texture ("shadowMap"));

	RenderLightObject::Shadow shadow = renderLightObject->GetShadow ();

	texture->SetSize (Size (shadow.resolution.x, shadow.resolution.y));
	texture->SetMipmapGeneration (false);
	texture->SetSizedInternalFormat (TEXTURE_SIZED_INTERNAL_FORMAT::FORMAT_DEPTH16);
	texture->SetInternalFormat (TEXTURE_INTERNAL_FORMAT::FORMAT_DEPTH);
	texture->SetChannelType (TEXTURE_CHANNEL_TYPE::CHANNEL_FLOAT);
	texture->SetWrapMode (TEXTURE_WRAP_MODE::WRAP_CLAMP_BORDER);
	texture->SetMinFilter (TEXTURE_FILTER_MODE::FILTER_LINEAR);
	texture->SetMagFilter (TEXTURE_FILTER_MODE::FILTER_LINEAR);
	texture->SetAnisotropicFiltering (false);
	texture->SetBorderColor (Color (glm::vec4 (1.0)));

	Resource<Framebuffer> framebuffer = Resource<Framebuffer> (new Framebuffer (nullptr, texture));

	_volume = new PerspectiveShadowMapVolume (framebuffer);

	_volume->SetLightCamera (new PerspectiveCamera ());
	_volume->SetShadowBias (shadow.bias);
}