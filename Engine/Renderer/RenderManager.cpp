#include "RenderManager.h"

#include <vector>
#include <algorithm>

#include "Renderer.h"

#include "SceneGraph/SceneObject.h"
#include "Skybox/Skybox.h"

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
// Render part
void RenderManager::RenderScene (Scene* scene)
{
	// Render the Skybox first
	Skybox::Render ();

	// Render other scnene elements
	std::vector<Renderer*> renderers;

	for (std::size_t i=0;i<scene->GetObjectsCount ();i++) {
		renderers.push_back (scene->GetObjectAt (i)->GetRenderer ());
	}

	std::sort (renderers.begin (), renderers.end (), cmp);

	for (Renderer* renderer : renderers) {
		renderer->Draw ();
	}
}
