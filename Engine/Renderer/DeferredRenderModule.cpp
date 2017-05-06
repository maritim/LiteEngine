#include "DeferredRenderModule.h"

#include <vector>
#include <algorithm>

#include "Renderer.h"

#include "Core/Intersections/Intersection.h"

#include "Skybox/Skybox.h"
#include "Lighting/LightsManager.h"
#include "Lighting/VolumetricLight.h"

#include "Pipeline.h"

#include "Wrappers/OpenGL/GL.h"

#include "Systems/Window/Window.h"
#include "Systems/Input/Input.h"

#include "Debug/Profiler/Profiler.h"
#include "Debug/Statistics/StatisticsManager.h"
#include "Debug/Statistics/DrawnObjectsCountStat.h"

DeferredRenderModule::DeferredRenderModule () :
	_frameBuffer (new GBuffer ())
{
	_frameBuffer->Init (Window::GetWidth (), Window::GetHeight ());
}

DeferredRenderModule::~DeferredRenderModule ()
{
	delete _frameBuffer;
}

bool cmp (Renderer* a, Renderer* b) {
	return a->GetPriority () < b->GetPriority ();
}

void DeferredRenderModule::RenderScene (Scene* scene, Camera* camera)
{
	/*
	 * Send Camera to Pipeline
	*/

	UpdateCamera (camera);

	/*
	 * Deferred Rendering Pass
	*/

	DeferredPass (scene, camera);

	/*
	 * Forward Rendering Pass
	*/

	ForwardPass (scene);
}

void DeferredRenderModule::DeferredPass (Scene* scene, Camera* camera)
{
	PROFILER_LOGGER ("Deferred Pass");

	/*
	 * Update GBuffer if needed
	*/

	UpdateGBuffer ();

	/*
	 * Deferred Rendering: Prepare for rendering
	*/

	PrepareDrawing ();

	/*
	 * Deferred Rendering: Geometry Pass
	*/

	GeometryPass (scene, camera);

	/*
	 * Deferred Rendering: Light Pass (atm)
	*/

	LightPass (scene, camera);

	/*
	 * Deferred Rendering: Decorations Pass
	*/

	SkyboxPass ();

	/*
	 * Deferred Rendering: End Drawing
	*/

	EndDrawing ();
}

void DeferredRenderModule::ForwardPass (Scene* scene)
{
	PROFILER_LOGGER("ForwardPass");

	/*
	 * Render scene entities to framebuffer at Forward Rendering Stage
	*/

	std::vector<Renderer*> renderers;

	for (std::size_t i=0;i<scene->GetObjectsCount ();i++) {
		if (scene->GetObjectAt (i)->GetRenderer ()->GetStageType () != Renderer::StageType::FORWARD_STAGE) {
			continue;
		}

		renderers.push_back (scene->GetObjectAt (i)->GetRenderer ());
	}

	std::sort (renderers.begin (), renderers.end (), cmp);

	for (Renderer* renderer : renderers) {
		renderer->Draw ();
	}
}

void DeferredRenderModule::UpdateGBuffer ()
{
	glm::ivec2 resizeEvent = Input::GetResizeEvent ();

	if (resizeEvent == glm::ivec2 (0)) {
		return ;
	}

	_frameBuffer->Clear ();
	_frameBuffer->Init ((int)resizeEvent.x, (int)resizeEvent.y);
}

void DeferredRenderModule::PrepareDrawing ()
{
	_frameBuffer->StartFrame ();
}

void DeferredRenderModule::GeometryPass (Scene* scene, Camera* camera)
{
	_frameBuffer->BindForGeomPass ();

	GL::Clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	/*
	 * Set Stencil Buffer to know where something is drawn in GBuffer
	 * Reference: http://www.gamedev.net/topic/570610-deferred-lighting---skybox-objects-without-light-/
	*/

	GL::Enable (GL_STENCIL_TEST);
	GL::StencilFunc (GL_ALWAYS, 1, 0xFF);
	GL::StencilOp (GL_KEEP, GL_KEEP, GL_REPLACE);

	/*
	 * Render scene entities to framebuffer at Deferred Rendering Stage
	*/

	std::vector<Renderer*> renderers;

	FrustumVolume* frustum = camera->GetFrustumVolume ();

	std::size_t drawnObjectsCount = 0;

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

		drawnObjectsCount ++;

		renderers.push_back (scene->GetObjectAt (i)->GetRenderer ());
	}

	StatisticsManager::Instance ()->SetStatisticsObject ("DrawnObjectsCount", new DrawnObjectsCountStat (drawnObjectsCount));

	std::sort (renderers.begin (), renderers.end (), cmp);

	for (Renderer* renderer : renderers) {
		renderer->Draw ();
	}

	/*
	 * Disable Stecil Test for further rendering
	*/

	GL::Disable (GL_STENCIL_TEST);
}

void DeferredRenderModule::LightPass (Scene* scene, Camera* camera)
{
	_frameBuffer->BindForLightPass();

	DirectionalLightPass (scene, camera);
	PointLightPass (scene, camera);
}

void DeferredRenderModule::DirectionalLightPass (Scene* scene, Camera* camera)
{
	GL::Disable(GL_DEPTH_TEST);
	GL::BlendFunc (GL_ONE, GL_ZERO);

	for (std::size_t i=0;i<LightsManager::Instance ()->GetDirectionalLightsCount ();i++) {
		VolumetricLight* volumetricLight = LightsManager::Instance ()->GetDirectionalLight (i);

		volumetricLight->GetLightRenderer ()->Draw (scene, camera, _frameBuffer);
	}

	GL::Disable(GL_BLEND);
	GL::Enable (GL_DEPTH_TEST);
}

void DeferredRenderModule::PointLightPass (Scene* scene, Camera* camera)
{
	for (std::size_t i=0;i<LightsManager::Instance ()->GetPointLightsCount ();i++) {
		VolumetricLight* volumetricLight = LightsManager::Instance ()->GetPointLight (i);

		PointLightStencilPass (volumetricLight);
		PointLightDrawPass (volumetricLight);
	}
}

void DeferredRenderModule::PointLightStencilPass (VolumetricLight* volumetricLight)
{
	_frameBuffer->BindForStencilPass ();

	/*
	 * Don't need to write the light on depth buffer.
	*/

	GL::Enable (GL_DEPTH_TEST);
	GL::DepthMask (GL_FALSE);

	/*
	 * Stencil pass technique. For more details please look at the link bellow.
	 * http://ogldev.atspace.co.uk/www/tutorial37/tutorial37.html
	 *
	 * It uses stencil test to identify the fragments that are in light. 
	*/

	GL::Enable (GL_STENCIL_TEST);
	GL::StencilFunc (GL_ALWAYS, 0, 0);

	GL::StencilOpSeparate (GL_BACK, GL_KEEP, GL_INCR_WRAP, GL_KEEP);
	GL::StencilOpSeparate (GL_FRONT, GL_KEEP, GL_DECR_WRAP, GL_KEEP);

	/*
	 * Both faces need to be drawn.
	*/

	GL::Disable (GL_CULL_FACE);

	/*
	 * Volumetric light draw.
	*/

	volumetricLight->GetLightRenderer ()->Draw (nullptr, nullptr);

	/*
	 * Reset the settings.
	*/

	GL::DepthMask (GL_TRUE);

	GL::Enable (GL_CULL_FACE);
	GL::Disable (GL_STENCIL_TEST);
}

void DeferredRenderModule::PointLightDrawPass (VolumetricLight* volumetricLight)
{
	_frameBuffer->BindForLightPass ();

	/*
	 * Don't need to write the light on depth buffer.
	*/

	GL::Disable (GL_DEPTH_TEST);
	GL::DepthMask (GL_FALSE);
	
	/*
	 * Pass the fragment only if it is like that
	 *
	 * 				(buffer frag) > (value '1')
	 *
	 * Don't override the sky, which have value '0'.
	 * Override only what fragments that are in light.
	 *
	 * After drawing, Stencil Buffer is refilled with 1's.
	*/

	GL::Enable (GL_STENCIL_TEST);
	GL::StencilFunc (GL_LESS, 1, 0xFF);
	GL::StencilOp (GL_KEEP, GL_KEEP, GL_REPLACE);

	/*
	 * Blend between point lights with same weight.
	*/

	GL::Enable (GL_BLEND);
	GL::BlendEquation (GL_FUNC_ADD);
	GL::BlendFunc (GL_ONE, GL_ONE);

	/*
	 * Process fragments single time. Front Face randomly chosen.
	*/

	GL::Enable (GL_CULL_FACE);
	GL::CullFace (GL_FRONT);

	/*
	 * Draw the volumetric light.
	*/

	volumetricLight->GetLightRenderer ()->Draw (nullptr, nullptr);

	/*
	 * Reset the settings.
	*/

	GL::CullFace (GL_BACK);

	GL::Disable (GL_STENCIL_TEST);
	GL::Disable (GL_BLEND);

	GL::DepthMask (GL_TRUE);
	GL::Enable (GL_DEPTH_TEST);
}

void DeferredRenderModule::SkyboxPass ()
{
	GL::Enable (GL_STENCIL_TEST);
	GL::StencilFunc (GL_EQUAL, 0, 0xFF);
	GL::StencilOp (GL_KEEP, GL_KEEP, GL_KEEP);

	Skybox::Render ();

	GL::Disable (GL_STENCIL_TEST);
}

void DeferredRenderModule::EndDrawing ()
{
	_frameBuffer->BindForFinalPass();

	std::size_t windowWidth = Window::GetWidth ();
	std::size_t windowHeight = Window::GetHeight ();

	GL::BlitFramebuffer(0, 0, windowWidth, windowHeight, 0, 0, windowWidth, windowHeight, 
		GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT, GL_NEAREST);
}

/*
 * TODO: Move this part somewhere else because it belongs to another
 * abstraction layer. This class only work with objects rendering, not
 * pipeline's job
*/
	
void DeferredRenderModule::UpdateCamera (Camera* camera)
{
	// Create Projection
	Pipeline::CreateProjection (camera);

	// Create View Matrix
	Pipeline::SendCamera (camera);	
}
