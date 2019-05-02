#include "GameBase.h"

#include "Managers/RenderSettingsManager.h"

#include "Resources/Resources.h"

extern "C" GameModule* CreateGameModule ()
{
	return new GameBase;
}

extern "C" void DestroyGameModule (GameModule* gameModule)
{
	delete gameModule;
}

void GameBase::Init ()
{
	std::string renderSettingsPath = "Assets/RenderSettings/Default.rsettings";
	RenderSettings* settings = Resources::LoadRenderSettings (renderSettingsPath);
	settings->renderMode = "DirectLightingRenderModule";

	RenderSettingsManager::Instance ()->AddRenderSettings (settings);
	RenderSettingsManager::Instance ()->SetActive (settings);
}

void GameBase::UpdateFrame ()
{
	/*
	 * Do nothing
	*/
}

void GameBase::UpdateScene ()
{
	/*
	 * Do nothing
	*/
}

void GameBase::RenderScene ()
{
	/*
	 * Do nothing
	*/
}
