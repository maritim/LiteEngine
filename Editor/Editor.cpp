#include "Editor.h"

#include <cstring>
#include <ImGui/ImGuizmo/ImGuizmo.h>

#include "Systems/Settings/SettingsManager.h"

#include "Managers/RenderSettingsManager.h"

#include "EditorManager.h"
#include "EditorScene.h"
#include "EditorGame.h"

void Editor::Init()
{
	LoadEditorLayout();

	EditorScene::Instance()->Init();
	EditorGame::Instance()->Init();

	RenderSettings* settings = new RenderSettings();
	settings->renderMode = "EditorRenderModule";

	RenderSettingsManager::Instance()->SetActive(settings);
}

void Editor::UpdateFrame()
{
	ImGuizmo::BeginFrame();
}

void Editor::UpdateScene()
{
	EditorManager::Instance()->Update();
	EditorScene::Instance()->Update();
	EditorGame::Instance()->Update();
}

void Editor::RenderScene()
{
	EditorScene::Instance()->Render();
	EditorGame::Instance()->Render();
}

void Editor::LoadEditorLayout()
{
	std::string layoutPath = SettingsManager::Instance()->GetValue<std::string>(
		"Layout", "layout_file_path", std::string()
		);

	if (layoutPath == std::string()) {
		return;
	}

	char* path = new char[layoutPath.size()];
	strcpy(path, layoutPath.c_str());
	ImGui::GetIO().IniFilename = path;
}