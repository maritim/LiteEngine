#include "RenderModuleManager.h"

RenderModuleManager::RenderModuleManager ()
{

}

RenderModuleManager::~RenderModuleManager ()
{
	for (auto renderModule : _renderModules) {
		renderModule.second->ClearModule ();

		delete renderModule.second;
	}
}

void RenderModuleManager::RegisterRenderModule (const std::string& name, RenderModule* renderModule)
{
	if (renderModule == nullptr) {
		return ;
	}

	if (_renderModules.find (name) != _renderModules.end ()) {
		return ;
	}

	_renderModules [name] = renderModule;
}

RenderModule* RenderModuleManager::GetRenderModule (const std::string& name)
{
	if (_renderModules.find (name) == _renderModules.end ()) {
		return nullptr;
	}

	return _renderModules [name];
}
