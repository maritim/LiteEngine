#include "RenderManager.h"

#include "RenderModuleManager.h"

#include "Debug/Profiler/Profiler.h"

/*
 * Singleton Part
*/

RenderManager::RenderManager () :
	_renderScene (nullptr)
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
	_renderScene = new RenderScene ();
}

void RenderManager::SetRenderSkyboxObject (RenderSkyboxObject* renderSkyboxObject)
{
	_renderScene->SetRenderSkyboxObject (renderSkyboxObject);
}

void RenderManager::AttachRenderObject (RenderObject* renderObject)
{
	_renderScene->AttachRenderObject (renderObject);
}

void RenderManager::DetachRenderObject (RenderObject* renderObject)
{
	_renderScene->DetachRenderObject (renderObject);
}

void RenderManager::AttachRenderDirectionalLightObject (RenderDirectionalLightObject* renderDirectionalLightObject)
{
	_renderScene->AttachRenderDirectionalLightObject (renderDirectionalLightObject);
}

void RenderManager::DetachRenderDirectionalLightObject (RenderDirectionalLightObject* renderDirectionalLightObject)
{
	_renderScene->DetachRenderDirectionalLightObject (renderDirectionalLightObject);
}

void RenderManager::AttachRenderPointLightObject (RenderPointLightObject* renderPointLightObject)
{
	_renderScene->AttachRenderPointLightObject (renderPointLightObject);
}

void RenderManager::DetachRenderPointLightObject (RenderPointLightObject* renderPointLightObject)
{
	_renderScene->DetachRenderPointLightObject (renderPointLightObject);
}

void RenderManager::AttachRenderSpotLightObject (RenderSpotLightObject* renderSpotLightObject)
{
	_renderScene->AttachRenderSpotLightObject (renderSpotLightObject);
}

void RenderManager::DetachRenderSpotLightObject (RenderSpotLightObject* renderSpotLightObject)
{
	_renderScene->DetachRenderSpotLightObject (renderSpotLightObject);
}

void RenderManager::SetRenderAmbientLightObject (RenderAmbientLightObject* renderAmbientLightObject)
{
	_renderScene->SetRenderAmbientLightObject (renderAmbientLightObject);
}

//TODO: Remove this
#include <set>

RenderProduct RenderManager::Render (const Camera* camera, const RenderSettings& settings)
{
	PROFILER_LOGGER("Render")

	RenderModule* renderModule = RenderModuleManager::Instance ()->GetRenderModule (settings.renderMode);

	static std::set<RenderModule*> initalized;

	if (initalized.find (renderModule) == initalized.end ()) {

		renderModule->InitModule (settings);

		initalized.insert (renderModule);
	}

	RenderProduct result = renderModule->Render (_renderScene, camera, settings);

	return result;
}

void RenderManager::Clear ()
{
	delete _renderScene;
}
