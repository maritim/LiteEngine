#include "Editor.h"

#include "Systems/GUI/ImGuizmo/ImGuizmo.h"

#include "Managers/SceneManager.h"
#include "Managers/RenderSettingsManager.h"

#include "EditorManager.h"
#include "EditorScene.h"

extern "C" __declspec(dllexport) GameModule* CreateGameModule ()
{
	return new Editor;
}

extern "C" __declspec(dllexport) void DestroyGameModule (GameModule* gameModule)
{
	delete gameModule;
}

void Editor::Init ()
{
	EditorScene::Instance ()->Init ();

	RenderSettings* settings = new RenderSettings ();
	settings->renderMode = "EditorRenderModule";

	RenderSettingsManager::Instance ()->SetActive (settings);
}

void Editor::UpdateFrame ()
{
	/*
	 * Start ImGuizmo frame
	*/

	ImGuizmo::BeginFrame();
}

void Editor::UpdateScene ()
{
	EditorManager::Instance ()->Update ();
	EditorScene::Instance ()->Update ();
}

void Editor::RenderScene ()
{
	EditorScene::Instance ()->Render (SceneManager::Instance ()->Current ());
}
