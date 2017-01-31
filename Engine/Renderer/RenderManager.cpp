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

#define VOLUME_DIMENSTIONS 256

/*
 * Singleton Part
*/

RenderManager::RenderManager () :
	_frameBuffer (new GBuffer ()),
	_volumeTexture (0)
{
	_frameBuffer->Init (Window::GetWidth (), Window::GetHeight ());

	// GL::Enable(GL_TEXTURE_3D);

	GL::GenTextures (1, &_volumeTexture);
	GL::BindTexture (GL_TEXTURE_3D, _volumeTexture);
	GL::TexImage3D(GL_TEXTURE_3D, 0, GL_RGBA8, 
		VOLUME_DIMENSTIONS, VOLUME_DIMENSTIONS, VOLUME_DIMENSTIONS, 
		0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
	GL::TexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	GL::TexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	GL::TexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	GL::TexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	GL::TexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_BORDER);

	// Create an fbo for clearing the 3D texture.
	// unsigned int volumeFbo;
	// GL::GenFramebuffers(1, &volumeFbo);
	// GL::BindFramebuffer(GL_FRAMEBUFFER, volumeFbo);
	// GL::FramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, _volumeTexture, 0);
	// GL::BindFramebuffer(GL_FRAMEBUFFER, 0);    
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

	VoxelizePass (scene);

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

	ForwardPass (scene);
}

#include "Debug/Logger/Logger.h"

void RenderManager::VoxelizePass (Scene* scene)
{
	PROFILER_LOGGER ("Voxelize Pass");

	ClearVoxels ();

	// Render to window but mask out all color.
	GL::ColorMask (GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
	GL::DepthMask (GL_FALSE);
	GL::Viewport (0, 0, VOLUME_DIMENSTIONS, VOLUME_DIMENSTIONS);

	// Load shader
	ShaderManager::Instance ()->AddShader ("VOXELIZATION_SHADER",
		"Assets/Shaders/Voxelize/voxelizeVertex.glsl",
		"Assets/Shaders/Voxelize/voxelizeFragment.glsl",
		"Assets/Shaders/Voxelize/voxelizeGeometry.glsl");

	// Use shader
	Pipeline::SetShader (ShaderManager::Instance ()->GetShader ("VOXELIZATION_SHADER"));

	GL::BindImageTexture (0, _volumeTexture, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA8);
	glUniform1i(0, 0);

	UpdateVoxelizationPipelineAttributes (scene);

	// Render
	for (std::size_t i=0;i<scene->GetObjectsCount ();i++) {
		if (scene->GetObjectAt (i)->GetRenderer ()->GetStageType () != Renderer::StageType::DEFERRED_STAGE) {
			continue;
		}

		scene->GetObjectAt (i)->GetRenderer ()->Draw ();
	}

	GL::ColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
	GL::DepthMask(GL_TRUE);
	GL::Viewport (0, 0, Window::GetWidth (), Window::GetHeight ());

	// unsigned int glBakeFbo;

	// glGenFramebuffers(1, &glBakeFbo);
	// glBindFramebuffer(GL_FRAMEBUFFER, glBakeFbo);
	// glBindFramebuffer(GL_READ_FRAMEBUFFER, glBakeFbo);

	GL::MemoryBarrier(GL_ALL_BARRIER_BITS);
	static char data[VOLUME_DIMENSTIONS * VOLUME_DIMENSTIONS * VOLUME_DIMENSTIONS * 4] = {0};
	glGetTexImage(GL_TEXTURE_3D, 0, GL_RED_INTEGER, GL_UNSIGNED_BYTE, data);

	DEBUG_LOG (std::to_string (data [0]));

	// int numBytes = VOLUME_DIMENSTIONS * VOLUME_DIMENSTIONS * VOLUME_DIMENSTIONS * 4;
	// unsigned char *pixels = (unsigned char*)malloc(numBytes); // allocate image data into RAM

	// glReadPixels( 0, 0, VOLUME_DIMENSTIONS, VOLUME_DIMENSTIONS, GL_RGBA , GL_UNSIGNED_BYTE , pixels);
	// free(pixels);
}

void RenderManager::UpdateVoxelizationPipelineAttributes (Scene* scene)
{
	AABBVolume* boundingBox = scene->GetBoundingBox ();
	AABBVolume::AABBVolumeInformation* volume = boundingBox->GetVolumeInformation ();

	std::vector<PipelineAttribute> attributes;

	PipelineAttribute minPosition;
	PipelineAttribute maxPosition;

	minPosition.type = PipelineAttribute::AttrType::ATTR_3F;
	maxPosition.type = PipelineAttribute::AttrType::ATTR_3F;

	minPosition.name = "minPosition";
	maxPosition.name = "maxPosition";

	minPosition.value = volume->minVertex;
	maxPosition.value = volume->maxVertex;

	attributes.push_back (minPosition);
	attributes.push_back (maxPosition);

	Pipeline::SendCustomAttributes ("VOXELIZATION_SHADER", attributes);
}

void RenderManager::VoxelRayTracePass ()
{
	PROFILER_LOGGER ("Ray Trace Pass");

	// Render Ox projection to window
	GL::Clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Load shader
	ShaderManager::Instance ()->AddShader ("VOXEL_RAY_TRACE_SHADER",
		"Assets/Shaders/Voxelize/voxelRayTraceVertex.glsl",
		"Assets/Shaders/Voxelize/voxelRayTraceFragment.glsl");

	// Use shader
	Pipeline::SetShader (ShaderManager::Instance ()->GetShader ("VOXEL_RAY_TRACE_SHADER"));

	UpdateVoxelRayTracePipelineAttributes ();

	// Render fullscreen quad.	
	glEnableVertexAttribArray(0);

	glBegin(GL_QUADS);
		glVertexAttrib2f(0, -1.0f, -1.0f);
		glVertexAttrib2f(0, 1.0f, -1.0f);
		glVertexAttrib2f(0, 1.0f, 1.0f);
		glVertexAttrib2f(0, -1.0f, 1.0f);
	glEnd();
}

void RenderManager::UpdateVoxelRayTracePipelineAttributes ()
{
	std::vector<PipelineAttribute> attributes;

	PipelineAttribute volumeTexture;

	volumeTexture.type = PipelineAttribute::AttrType::ATTR_TEXTURE_3D;

	volumeTexture.name = "volumeTexture";

	volumeTexture.value.x = _volumeTexture;

	attributes.push_back (volumeTexture);

	Pipeline::SendCustomAttributes ("VOXEL_RAY_TRACE_SHADER", attributes);	
}

void RenderManager::ClearVoxels ()
{
	unsigned int volumeFbo;
	GL::GenFramebuffers(1, &volumeFbo);
	GL::BindFramebuffer(GL_FRAMEBUFFER, volumeFbo);
	GL::FramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, _volumeTexture, 0);
	GL::BindFramebuffer(GL_FRAMEBUFFER, 0);
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
	glm::vec3 resizeEvent = Input::GetResizeEvent ();

	if (resizeEvent == glm::vec3 (0.0f)) {
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
