#include "GameBase.h"

#include "Managers/RenderSettingsManager.h"

#include "Resources/Resources.h"

void GameBase::Init ()
{
	std::string renderSettingsPath = "Assets/RenderSettings/Default.rsettings";
	RenderSettings* settings = Resources::LoadRenderSettings (renderSettingsPath);

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
