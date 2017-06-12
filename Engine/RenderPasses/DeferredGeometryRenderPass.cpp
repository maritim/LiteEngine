#include "DeferredGeometryRenderPass.h"

#include <algorithm>

#include "Systems/Window/Window.h"
#include "Systems/Input/Input.h"

#include "Core/Intersections/Intersection.h"

#include "Renderer/Pipeline.h"

#include "Wrappers/OpenGL/GL.h"

#include "Debug/Profiler/Profiler.h"
#include "Debug/Statistics/StatisticsManager.h"
#include "Debug/Statistics/DrawnObjectsCountStat.h"

DeferredGeometryRenderPass::DeferredGeometryRenderPass () :
	_frameBuffer (new GBuffer ())
{

}

DeferredGeometryRenderPass::~DeferredGeometryRenderPass ()
{
	delete _frameBuffer;
}

void DeferredGeometryRenderPass::Init ()
{
	_frameBuffer->Init (Window::GetWidth (), Window::GetHeight ());
}

RenderVolumeCollection* DeferredGeometryRenderPass::Execute (Scene* scene, Camera* camera, RenderVolumeCollection* rvc)
{
	/*
	* Send Camera to Pipeline
	*/

	UpdateCamera (camera);

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

	return rvc->Insert ("GBuffer", _frameBuffer);
}

void DeferredGeometryRenderPass::UpdateGBuffer ()
{
	glm::ivec2 resizeEvent = Input::GetResizeEvent ();

	if (resizeEvent == glm::ivec2 (0)) {
		return;
	}

	_frameBuffer->Clear ();
	_frameBuffer->Init ((int) resizeEvent.x, (int) resizeEvent.y);
}

void DeferredGeometryRenderPass::PrepareDrawing ()
{
	_frameBuffer->StartFrame ();
}

bool cmp (Renderer* a, Renderer* b) {
	return a->GetPriority () < b->GetPriority ();
}

void DeferredGeometryRenderPass::GeometryPass (Scene* scene, Camera* camera)
{
	_frameBuffer->BindForGeomPass ();

	GL::Clear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	/*
	* Set Stencil Buffer to know where something is drawn in GBuffer
	* Reference: http://www.gamedev.net/topic/570610-deferred-lighting---skybox-objects-without-light-/
	*/

	GL::Enable (GL_STENCIL_TEST);
	GL::StencilFunc (GL_ALWAYS, 1, 0xFF);
	GL::StencilOp (GL_KEEP, GL_KEEP, GL_REPLACE);

	/*
	* Enable Back Face Culling
	*/

	GL::Enable (GL_CULL_FACE);
	GL::CullFace (GL_BACK);

	/*
	* Render scene entities to framebuffer at Deferred Rendering Stage
	*/

	std::vector<Renderer*> renderers;

	FrustumVolume* frustum = camera->GetFrustumVolume ();

	std::size_t drawnObjectsCount = 0;

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

		drawnObjectsCount++;

		renderers.push_back (sceneObject->GetRenderer ());
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

/*
* TODO: Move this part somewhere else because it belongs to another
* abstraction layer. This class only work with objects rendering, not
* pipeline's job
*/

void DeferredGeometryRenderPass::UpdateCamera (Camera* camera)
{
	// Create Projection
	Pipeline::CreateProjection (camera);

	// Create View Matrix
	Pipeline::SendCamera (camera);
}
