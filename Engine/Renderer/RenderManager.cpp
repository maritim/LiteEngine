#include "RenderManager.h"

#include "RenderModules/DirectLightingRenderModule.h"
#include "RenderModules/LightMapRenderModule.h"
#include "RenderModules/VoxelizationRenderModule.h"
#include "RenderModules/VoxelConeTraceRenderModule.h"
#include "RenderModules/ReflectiveShadowMapRenderModule.h"

/*
 * Singleton Part
*/

RenderManager::RenderManager () :
	_currentRenderModule (nullptr),
	_currentRenderMode (RENDER_MODE_DIRECT_LIGHTING),
	_needToChangeRenderMode (false)
{

}

RenderManager::~RenderManager ()
{

}

/*
 * Render Part
*/

void RenderManager::Init ()
{
	_renderModules.push_back (new DirectLightingRenderModule ());
	_renderModules.push_back (new LightMapRenderModule ());
	_renderModules.push_back (new VoxelizationRenderModule ());
	_renderModules.push_back (new VoxelConeTraceRenderModule ());
	_renderModules.push_back (new ReflectiveShadowMapRenderModule ());

	_renderModules [(std::size_t) _currentRenderMode]->InitModule ();

	_currentRenderModule = _renderModules [(std::size_t) _currentRenderMode];
}

void RenderManager::SetRenderMode (RenderMode renderMode)
{
	_needToChangeRenderMode = true;

	_currentRenderMode = renderMode;
}

RenderMode RenderManager::GetRenderMode () const
{
	return _currentRenderMode;
}

void RenderManager::RenderScene (const Scene* scene, const Camera* camera)
{
	_currentRenderModule->RenderScene (scene, camera);

	if (_needToChangeRenderMode) {

		SwitchRenderModule ();

		_needToChangeRenderMode = false;
	}
}

void RenderManager::Clear ()
{
	_currentRenderModule->ClearModule ();

	for (auto renderModule : _renderModules) {
		delete renderModule;
	}

	_renderModules.clear ();
}

void RenderManager::SwitchRenderModule ()
{
	_currentRenderModule->ClearModule ();

	_renderModules [(std::size_t) _currentRenderMode]->InitModule ();

	_currentRenderModule = _renderModules [(std::size_t) _currentRenderMode];
}