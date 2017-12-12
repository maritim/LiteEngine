#include "RenderManager.h"

#include "Pipeline.h"

#include "RenderModules/DeferredRenderModule.h"
#include "RenderModules/VoxelizationRenderModule.h"
#include "RenderModules/VoxelConeTraceRenderModule.h"
#include "RenderModules/ReflectiveShadowMapRenderModule.h"

/*
 * Singleton Part
*/

RenderManager::RenderManager () :
	_currentRenderModule (nullptr),
	_currentRenderMode (RENDER_MODE_DEFERRED)
{
	_renderModules.push_back (new DeferredRenderModule ());
	_renderModules.push_back (new VoxelizationRenderModule ());
	_renderModules.push_back (new VoxelConeTraceRenderModule ());
	_renderModules.push_back (new ReflectiveShadowMapRenderModule ());

	for (auto renderModule : _renderModules) {
		renderModule->InitModule ();
	}

	SetRenderMode (_currentRenderMode);
}

RenderManager::~RenderManager ()
{
	Clear ();
}

/*
 * Render Part
*/

void RenderManager::SetRenderMode (RenderMode renderMode)
{
	_currentRenderMode = renderMode;
	_currentRenderModule = _renderModules [(std::size_t) renderMode];
}

RenderMode RenderManager::GetRenderMode () const
{
	return _currentRenderMode;
}

void RenderManager::RenderScene (Scene* scene, Camera* camera)
{
	_currentRenderModule->RenderScene (scene, camera);
}

void RenderManager::Clear ()
{
	for (auto renderModule : _renderModules) {
		delete renderModule;
	}

	_renderModules.clear ();
}
