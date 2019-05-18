#include "RenderManager.h"

#include "RenderModuleManager.h"

/*
 * Singleton Part
*/

RenderManager::RenderManager ()
{

}

RenderManager::~RenderManager ()
{

}

SPECIALIZE_SINGLETON(RenderManager)

/*
 * Render Part
*/

void RenderManager::Init ()
{

}

//TODO: Remove this
#include <set>

RenderProduct RenderManager::RenderScene (const Scene* scene, const Camera* camera, const RenderSettings& settings)
{
	RenderModule* renderModule = RenderModuleManager::Instance ()->GetRenderModule (settings.renderMode);

	static std::set<RenderModule*> initalized;

	if (initalized.find (renderModule) == initalized.end ()) {

		renderModule->InitModule (settings);

		initalized.insert (renderModule);
	}

	RenderProduct result = renderModule->RenderScene (scene, camera, settings);

	return result;
}

void RenderManager::Clear ()
{

}
