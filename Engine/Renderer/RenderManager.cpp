#include "RenderManager.h"

#include <vector>
#include <algorithm>

#include "Renderer.h"

#include "SceneGraph/SceneObject.h"
#include "Skybox/Skybox.h"

#include "Pipeline.h"

// Singleton part
RenderManager::RenderManager ()
{

}

RenderManager::~RenderManager ()
{
	
}

bool cmp (Renderer* a, Renderer* b) {
	return a->GetPriority () < b->GetPriority ();
}

#include "Core/Debug/Debug.h"

// Render part
void RenderManager::RenderScene (Scene* scene, Camera* camera)
{
	// TODO: Move this part somewhere else because it belongs to another
	// abstraction layer. This class only work with objects rendering, not
	// pipeline's job
	
	// Create Perspective Projection
	if (camera->GetType () == Camera::Type::PERSPECTIVE) {
		Pipeline::CreatePerspective (camera->GetFieldOfView (),
			camera->GetAspect (), camera->GetZNear (), camera->GetZFar ());
	} else {
		//TODO: Add Orthographic 
	}

	// Create View Matrix
	Pipeline::SendCamera (camera);

	// Render the Skybox first
	Skybox::Render ();

	// Render other scene elements
	std::vector<Renderer*> renderers;

	for (std::size_t i=0;i<scene->GetObjectsCount ();i++) {
		renderers.push_back (scene->GetObjectAt (i)->GetRenderer ());
	}

	std::sort (renderers.begin (), renderers.end (), cmp);

	for (Renderer* renderer : renderers) {
		renderer->Draw ();
	}
}
