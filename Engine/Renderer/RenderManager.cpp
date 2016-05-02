#include "RenderManager.h"

#include <vector>
#include <algorithm>

#include "Renderer.h"

#include "SceneGraph/SceneObject.h"
#include "Skybox/Skybox.h"

#include "Pipeline.h"

#include "Wrappers/OpenGL/GL.h"

#include "Systems/Window/Window.h"

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
	 * Render the Skybox first
	*/

	Skybox::Render ();

	/*
	 * Deffered Rendering: Geometry Pass
	*/

	GeometryPass (scene);

	/*
	 * Deffered Rendering: Light Pass (atm)
	*/

	LightPass ();
}

void RenderManager::GeometryPass (Scene* scene)
{
	_frameBuffer->BindForWriting ();
	
	GL::Clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 

	/*
	 * Render scene elements to framebuffer
	*/

	std::vector<Renderer*> renderers;

	for (std::size_t i=0;i<scene->GetObjectsCount ();i++) {
		renderers.push_back (scene->GetObjectAt (i)->GetRenderer ());
	}

	std::sort (renderers.begin (), renderers.end (), cmp);

	for (Renderer* renderer : renderers) {
		renderer->Draw ();
	}
} 

void RenderManager::LightPass ()
{
	GL::BindFramebuffer(GL_FRAMEBUFFER, 0);

	GL::Clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	_frameBuffer->BindForReading();

	std::size_t windowWidth = Window::GetWidth ();
	std::size_t windowHeight = Window::GetHeight ();

	GLsizei HalfWidth = (GLsizei)(windowWidth / 2.0f);
	GLsizei HalfHeight = (GLsizei)(windowHeight / 2.0f);

	_frameBuffer->SetReadBuffer(GBuffer::GBUFFER_TEXTURE_TYPE_POSITION);
	GL::BlitFramebuffer(0, 0, windowWidth, windowHeight,
		0, 0, HalfWidth, HalfHeight, GL_COLOR_BUFFER_BIT, GL_LINEAR);

	_frameBuffer->SetReadBuffer(GBuffer::GBUFFER_TEXTURE_TYPE_DIFFUSE);
	GL::BlitFramebuffer(0, 0, windowWidth, windowHeight,
		0, HalfHeight, HalfWidth, windowHeight, GL_COLOR_BUFFER_BIT, GL_LINEAR);

	_frameBuffer->SetReadBuffer(GBuffer::GBUFFER_TEXTURE_TYPE_NORMAL);
	GL::BlitFramebuffer(0, 0, windowWidth, windowHeight,
		HalfWidth, HalfHeight, windowWidth, windowHeight, GL_COLOR_BUFFER_BIT, GL_LINEAR);

	_frameBuffer->SetReadBuffer(GBuffer::GBUFFER_TEXTURE_TYPE_TEXCOORD);
	GL::BlitFramebuffer(0, 0, windowWidth, windowHeight,
		HalfWidth, 0, windowWidth, HalfHeight, GL_COLOR_BUFFER_BIT, GL_LINEAR);
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
