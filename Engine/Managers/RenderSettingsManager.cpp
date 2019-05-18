#include "RenderSettingsManager.h"

RenderSettingsManager::RenderSettingsManager () :
	_settings (),
	_active (nullptr)
{

}

RenderSettingsManager::~RenderSettingsManager ()
{

}

SPECIALIZE_SINGLETON(RenderSettingsManager)

void RenderSettingsManager::AddRenderSettings (RenderSettings* settings)
{
	if (_settings.find (settings->name) != _settings.end ()) {
		return ;
	}

	_settings [settings->name] = settings;
}

RenderSettings* RenderSettingsManager::GetRenderSettings (const std::string& filename)
{
	if (_settings.find (filename) == _settings.end ()) {
		return nullptr;
	}

	return _settings [filename];
}

void RenderSettingsManager::SetActive (RenderSettings* settings)
{
	_active = settings;
}

RenderSettings* RenderSettingsManager::GetActive () const
{
	return _active;
}
