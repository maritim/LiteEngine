#include "RenderManager.h"

#include <vector>
#include <algorithm>

#include "Renderer.h"

#include "SceneGraph/SceneObject.h"
#include "Skybox/Skybox.h"
#include "Lighting/LightsManager.h"
#include "Lighting/VolumetricLight.h"

#include "Pipeline.h"

#include "Wrappers/OpenGL/GL.h"

#include "Systems/Window/Window.h"
#include "Systems/Input/Input.h"

/*
 * Singleton Part
*/

RenderManager::RenderManager () :
	_frameBuffer (new GBuffer ())
{
	_frameBuffer->Init (Window::GetWidth (), Window::GetHeight ());
}

RenderManager::~RenderManager ()
{

}

bool cmp (Renderer* a, Renderer* b) {
	return a->GetPriority () < b->GetPriority ();
}

/*
 * Render Part
*/

void RenderManager::RenderScene (Scene* scene, Camera* camera)
{
	/*
	 * Send Camera to Pipeline
	*/

	UpdateCamera (camera);

	/*
	 * Deferred Rendering Pass
	*/

	DeferredPass (scene);

	/*
	 * Forward Rendering Pass
	*/

	ForwardPass (scene);
}	

void RenderManager::DeferredPass (Scene* scene)
{
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

	GeometryPass (scene);

	/*
	 * Deferred Rendering: Light Pass (atm)
	*/

	LightPass ();

	/*
	 * Deferred Rendering: Decorations Pass
	*/

	SkyboxPass ();
	// ParticleSystemPass ();
	// GUIPass ();

	/*
	 * Deferred Rendering: End Drawing
	*/

	EndDrawing ();
}

void RenderManager::ForwardPass (Scene* scene)
{
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

void RenderManager::UpdateGBuffer ()
{
	Vector3 resizeEvent = Input::GetResizeEvent ();

	if (resizeEvent == Vector3::Zero) {
		return ;
	}

	_frameBuffer->Clear ();
	_frameBuffer->Init ((int)resizeEvent.x, (int)resizeEvent.y);
}

void RenderManager::PrepareDrawing ()
{
	_frameBuffer->StartFrame ();
}

void RenderManager::GeometryPass (Scene* scene)
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

	for (std::size_t i=0;i<scene->GetObjectsCount ();i++) {
		if (scene->GetObjectAt (i)->GetRenderer ()->GetStageType () != Renderer::StageType::DEFERRED_STAGE) {
			continue;
		}

		renderers.push_back (scene->GetObjectAt (i)->GetRenderer ());
	}

	std::sort (renderers.begin (), renderers.end (), cmp);

	for (Renderer* renderer : renderers) {
		renderer->Draw ();
	}

	/*
	 * Disable Stecil Test for further rendering
	*/

	GL::Disable (GL_STENCIL_TEST);
}

void RenderManager::LightPass ()
{
	_frameBuffer->BindForLightPass();

	DirectionalLightPass ();
	PointLightPass ();
}

void RenderManager::DirectionalLightPass ()
{
	GL::Disable(GL_DEPTH_TEST);
	GL::BlendFunc (GL_ONE, GL_ZERO);

	for (std::size_t i=0;i<LightsManager::Instance ()->GetDirectionalLightsCount ();i++) {
		VolumetricLight* volumetricLight = LightsManager::Instance ()->GetDirectionalLight (i);

		volumetricLight->GetLightRenderer ()->Draw ();
	}

	GL::Disable(GL_BLEND);
	GL::Enable (GL_DEPTH_TEST);
}

void RenderManager::PointLightPass ()
{
	PointLightStencilPass ();
	PointLightDrawPass ();
}

void RenderManager::PointLightStencilPass ()
{

}

void RenderManager::PointLightDrawPass ()
{
	// glStencilFunc(GL_NOTEQUAL, 0, 0xFF);

	// GL::Disable(GL_DEPTH_TEST);
	// GL::Enable(GL_BLEND);
	// glBlendEquation(GL_FUNC_ADD);
	// GL::BlendFunc(GL_ONE, GL_ONE);

	// GL::Enable(GL_CULL_FACE);
	// glCullFace(GL_FRONT);

	// for (std::size_t i=0;i<LightsManager::Instance ()->GetPointLightsCount ();i++) {
	// 	VolumetricLight* volumetricLight = LightsManager::Instance ()->GetPointLight (i);

	// 	volumetricLight->GetLightRenderer ()->Draw ();
	// }

	// glCullFace(GL_BACK);

	// GL::Disable(GL_BLEND);	
}

void RenderManager::SkyboxPass ()
{
	GL::Enable (GL_STENCIL_TEST);
	GL::StencilFunc (GL_EQUAL, 0, 0xFF);
	GL::StencilOp (GL_KEEP, GL_KEEP, GL_KEEP);

	Skybox::Render ();

	GL::Disable (GL_STENCIL_TEST);
}

void RenderManager::EndDrawing ()
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
	
void RenderManager::UpdateCamera (Camera* camera)
{
	// Create Perspective Projection
	if (camera->GetType () == Camera::Type::PERSPECTIVE) {
		Pipeline::CreatePerspective (camera->GetFieldOfView (),
			camera->GetAspect (), camera->GetZNear (), camera->GetZFar ());
	} else {
		//TODO: Add Orthographic 
	}

	// Create View Matrix
	Pipeline::SendCamera (camera);	
}
