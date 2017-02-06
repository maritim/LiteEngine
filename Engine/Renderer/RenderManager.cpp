#include "RenderManager.h"

#include <vector>
#include <algorithm>

#include "Renderer.h"

#include "Core/Intersections/Intersection.h"

#include "SceneGraph/SceneObject.h"
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

#include "Managers/ShaderManager.h"

#include "Debug/Logger/Logger.h"
#include "Core/Console/Console.h"
#include "Core/Math/glm/gtx/string_cast.hpp"

#define VOLUME_DIMENSTIONS 512

/*
 * Singleton Part
*/

RenderManager::RenderManager () :
	_frameBuffer (new GBuffer ()),
	_voxelVolume (new VoxelVolume ())
{
	_frameBuffer->Init (Window::GetWidth (), Window::GetHeight ());
	_voxelVolume->Init (VOLUME_DIMENSTIONS);
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
	 * Voxelize the scene
	*/

	static bool firstTime = true;

	if (firstTime) {
		VoxelizePass(scene);
		firstTime = false;
	}

	/*
	 * Voxel Ray Trace Pass
	*/

	VoxelRayTracePass ();

	// /*
	//  * Deferred Rendering Pass
	// */

	// DeferredPass (scene, camera);

	/*
	 * Forward Rendering Pass
	*/

	// ForwardPass (scene);
}

void RenderManager::VoxelizePass (Scene* scene)
{
	PROFILER_LOGGER ("Voxelize Pass");

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

void RenderManager::StartVoxelization()
{
	_voxelVolume->ClearVoxels();

	_voxelVolume->StartVoxelizationPass ();
}

void RenderManager::GeometryVoxelizationPass(Scene* scene)
{
	/*
	 * Update voxel volume based on scene bounding box
	*/

	UpdateVoxelVolumeBoundingBox (scene);

	/*
	 * Bind voxel volume to geometry render pass
	*/

	_voxelVolume->BindForVoxelizationPass();

	/*
	 * Render geometry 
	*/
	
	for (std::size_t i = 0; i<scene->GetObjectsCount(); i++) {
		if (scene->GetObjectAt(i)->GetRenderer()->GetStageType() != Renderer::StageType::DEFERRED_STAGE) {
			continue;
		}

		scene->GetObjectAt(i)->GetRenderer()->Draw();
	}
}

void RenderManager::EndVoxelization()
{
	_voxelVolume->EndVoxelizationPass ();
}

void RenderManager::UpdateVoxelVolumeBoundingBox(Scene* scene)
{
	AABBVolume* boundingBox = scene->GetBoundingBox();
	AABBVolume::AABBVolumeInformation* volume = boundingBox->GetVolumeInformation();

	glm::vec3 minVertex = volume->minVertex;
	glm::vec3 maxVertex = volume->maxVertex;

	_voxelVolume->UpdateBoundingBox (minVertex, maxVertex);
}

void RenderManager::VoxelRayTracePass ()
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

void RenderManager::StartRayTrace()
{
	_voxelVolume->StartRayTracePass ();
}

void RenderManager::VoxelRenderingRayTracePass()
{
	_voxelVolume->BindForRayTracePass ();

	/*
	 * Clear framebuffer
	*/

	GL::Clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	/*
	 * Render fullscreen quad.	
	*/

	GL::EnableVertexAttribArray(0);

	GL::Begin(GL_QUADS);
		GL::VertexAttrib2f(0, -1.0f, -1.0f);
		GL::VertexAttrib2f(0, 1.0f, -1.0f);
		GL::VertexAttrib2f(0, 1.0f, 1.0f);
		GL::VertexAttrib2f(0, -1.0f, 1.0f);
	GL::End();
}

void RenderManager::DeferredPass (Scene* scene, Camera* camera)
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

	LightPass ();

	/*
	 * Deferred Rendering: Decorations Pass
	*/

	SkyboxPass ();

	/*
	 * Deferred Rendering: End Drawing
	*/

	EndDrawing ();
}

void RenderManager::ForwardPass (Scene* scene)
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

void RenderManager::UpdateGBuffer ()
{
	glm::ivec2 resizeEvent = Input::GetResizeEvent ();

	if (resizeEvent == glm::ivec2 (0)) {
		return ;
	}

	_frameBuffer->Clear ();
	_frameBuffer->Init ((int)resizeEvent.x, (int)resizeEvent.y);
}

void RenderManager::PrepareDrawing ()
{
	_frameBuffer->StartFrame ();
}

void RenderManager::GeometryPass (Scene* scene, Camera* camera)
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
	for (std::size_t i=0;i<LightsManager::Instance ()->GetPointLightsCount ();i++) {
		VolumetricLight* volumetricLight = LightsManager::Instance ()->GetPointLight (i);

		PointLightStencilPass (volumetricLight);
		PointLightDrawPass (volumetricLight);
	}
}

void RenderManager::PointLightStencilPass (VolumetricLight* volumetricLight)
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

	volumetricLight->GetLightRenderer ()->Draw ();

	/*
	 * Reset the settings.
	*/

	GL::DepthMask (GL_TRUE);

	GL::Enable (GL_CULL_FACE);
	GL::Disable (GL_STENCIL_TEST);
}

void RenderManager::PointLightDrawPass (VolumetricLight* volumetricLight)
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

	volumetricLight->GetLightRenderer ()->Draw ();

	/*
	 * Reset the settings.
	*/

	GL::CullFace (GL_BACK);

	GL::Disable (GL_STENCIL_TEST);
	GL::Disable (GL_BLEND);

	GL::DepthMask (GL_TRUE);
	GL::Enable (GL_DEPTH_TEST);
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
