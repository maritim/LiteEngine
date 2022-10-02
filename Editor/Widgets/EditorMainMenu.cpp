#include "EditorMainMenu.h"

#include <filesystem>
#include <ImGui/imgui.h>
#include <ImGui/imguifilesystem/imguifilesystem.h>

#include "Managers/SceneManager.h"

#include "EditorScene.h"
#include "EditorSelection.h"

#include <cstdlib>

#include "Systems/Input/Input.h"
#include "Systems/Settings/SettingsManager.h"

#include "Resources/SceneSaver.h"

#include "Utils/Files/FileSystem.h"

namespace fs = std::filesystem;

void EditorMainMenu::Show ()
{
	ShowMainMenu ();
}

bool ButtonCenteredOnLine(const char* label, float alignment = 0.45f)
{
	ImGuiStyle& style = ImGui::GetStyle();

	float size = ImGui::CalcTextSize(label).x + style.FramePadding.x * 2.0f;
	float avail = ImGui::GetContentRegionAvail().x;

	float off = (avail - size) * alignment;
	if (off > 0.0f)
		ImGui::SetCursorPosX(ImGui::GetCursorPosX() + off);

	return ImGui::Button(label);
}

void EditorMainMenu::ShowMainMenu ()
{
	if (ImGui::BeginMainMenuBar())
	{
		ShowMainMenuFile ();

		if (ImGui::BeginMenu("Edit"))
		{
			if (ImGui::MenuItem("Undo", "CTRL+Z")) {}
			if (ImGui::MenuItem("Redo", "CTRL+Y", false, false)) {}  // Disabled item
			ImGui::Separator();
			if (ImGui::MenuItem("Cut", "CTRL+X")) {}
			if (ImGui::MenuItem("Copy", "CTRL+C")) {}
			if (ImGui::MenuItem("Paste", "CTRL+V")) {}
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Window")) {

			bool lastShowHierarchy = SettingsManager::Instance ()->GetValue<bool> ("Menu", "show_hierarchy", false);
			bool showHierarchy = lastShowHierarchy;
			ImGui::MenuItem("Hierarchy Window", "CTRL+1", &showHierarchy);

			bool lastShowInspector = SettingsManager::Instance ()->GetValue<bool> ("Menu", "show_inspector", false);
			bool showInspector = lastShowInspector;
			ImGui::MenuItem ("Inspector Window", "CTRL+2", &showInspector);

			bool lastShowProject = SettingsManager::Instance ()->GetValue<bool> ("Menu", "show_project", false);
			bool showProject = lastShowProject;
			ImGui::MenuItem ("Project Window", "CTRL+3", &showProject);

			bool lastShowConsole = SettingsManager::Instance ()->GetValue<bool> ("Menu", "show_console", false);
			bool showConsole = lastShowConsole;
			ImGui::MenuItem ("Console Window", "CTRL+4", &showConsole);

			ImGui::Separator ();

			bool lastShowAnimationSettings = SettingsManager::Instance ()->GetValue<bool> ("Menu", "show_animation_settings", false);
			bool showAnimationSettings = lastShowAnimationSettings;
			ImGui::MenuItem("Animation Settings", "CTRL+5", &showAnimationSettings);

			ImGui::Separator ();

			bool lastShowRenderingSettings = SettingsManager::Instance ()->GetValue<bool> ("Menu", "show_rendering_settings", false);
			bool showRenderingSettings = lastShowRenderingSettings;
			ImGui::MenuItem("Rendering Settings", "CTRL+6", &showRenderingSettings);

			ImGui::Separator ();

			bool lastShowProfiler = SettingsManager::Instance ()->GetValue<bool> ("Menu", "show_profiler", false);
			bool showProfiler = lastShowProfiler;
			ImGui::MenuItem("Profiler", "CTRL+7", &showProfiler);

			ImGui::Separator();

			if (showHierarchy != lastShowHierarchy) {
				SettingsManager::Instance ()->SetValue<bool> ("Menu", "show_hierarchy", showHierarchy);
			}

			if (showInspector != lastShowInspector) {
				SettingsManager::Instance ()->SetValue<bool> ("Menu", "show_inspector", showInspector);
			}

			if (showProject != lastShowProject) {
				SettingsManager::Instance ()->SetValue<bool> ("Menu", "show_project", showProject);
			}

			if (showConsole != lastShowConsole) {
				SettingsManager::Instance ()->SetValue<bool> ("Menu", "show_console", showConsole);
			}

			if (showAnimationSettings != lastShowAnimationSettings) {
				SettingsManager::Instance ()->SetValue<bool> ("Menu", "show_animation_settings", showAnimationSettings);
			}

			if (showRenderingSettings != lastShowRenderingSettings) {
				SettingsManager::Instance ()->SetValue<bool> ("Menu", "show_rendering_settings", showRenderingSettings);
			}

			if (showProfiler != lastShowProfiler) {
				SettingsManager::Instance ()->SetValue<bool> ("Menu", "show_profiler", showProfiler);
			}

			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Help")) {
			bool lastShowAbout = SettingsManager::Instance()->GetValue<bool>("Menu", "show_about", false);
			bool showAbout = lastShowAbout;
			ImGui::MenuItem("About", "", &showAbout);

			if (showAbout != lastShowAbout) {
				SettingsManager::Instance()->SetValue<bool>("Menu", "show_about", showAbout);
			}

			if (ImGui::MenuItem("Open github repo"))
			{
				system("start https://github.com/almartdev/insiderengine");
			}
			ImGui::EndMenu();
		}

		ImGui::Separator ();
		
		if (EditorScene::Instance ()->IsActive () == false) {
			if (ButtonCenteredOnLine("Play")) {
				EditorScene::Instance ()->SetActive (true);
			}
		} else {
			if (ButtonCenteredOnLine("Stop")) {
				EditorScene::Instance ()->SetActive (false);
			}
		}

		ImGui::EndMainMenuBar();
	}
}



void EditorMainMenu::ShowMainMenuFile ()
{
	bool createScene = false;
	bool openScene = false;
	bool saveScene = false;
	bool saveScene2 = false;

	if (ImGui::BeginMenu("File"))
	{
		createScene = ImGui::MenuItem("New Scene", "Ctrl+N");
		
		openScene = ImGui::MenuItem("Open Scene", "Ctrl+O");

		saveScene = ImGui::MenuItem("Save Scene", "Ctrl+S");
		saveScene2 = ImGui::MenuItem("Save Scene As..", "Ctrl+Shift+S");

		bool lastShowImporter = SettingsManager::Instance()->GetValue<bool>("Menu", "show_packageimp", false);
		bool showImporter = lastShowImporter;
		ImGui::MenuItem("Import package", NULL, &showImporter);

		if (showImporter != lastShowImporter) {
			ImGui::SetNextWindowSize(ImVec2(550, 130));
			SettingsManager::Instance()->SetValue<bool>("Menu", "show_packageimp", showImporter);
		}

		ImGui::Separator();

		ImGui::EndMenu();
	}

	// Create new scene
	if (createScene == true || (Input::GetKey (InputKey::LCTRL) && Input::GetKeyDown (InputKey::N))) {
		EditorSelection::Instance ()->SetActive (nullptr);
		SceneManager::Instance ()->CreateScene ();
	}

	// Load scene
	static ImGuiFs::Dialog dialog;

	openScene = openScene || (Input::GetKey (InputKey::LCTRL) && Input::GetKeyDown (InputKey::O));

	const char* path = dialog.chooseFileDialog (openScene, "Assets/Scenes/", ".scene");

	if (strlen (path) > 0) {
		EditorSelection::Instance ()->SetActive (nullptr);

		std::string scenePath = FileSystem::Relative (path, fs::current_path ().string ());
		SceneManager::Instance ()->Load (scenePath);
		SettingsManager::Instance ()->SetValue<std::string> ("Scene", "scene_path", scenePath);
	}

	// Save scene
	if (saveScene || (Input::GetKey (InputKey::LCTRL) && Input::GetKeyDown (InputKey::S))) {
		auto& path = SceneManager::Instance ()->Current ()->GetPath ();

		if (path == std::string ()) {
			saveScene2 = true;
		} else {
			SceneSaver::Instance ().Save (SceneManager::Instance ()->Current (), path);		
		}
	}

	// Save As
	static ImGuiFs::Dialog dialog2;

	saveScene2 = saveScene2 || (Input::GetKey (InputKey::LCTRL) &&
		Input::GetKey (InputKey::LSHIFT) && Input::GetKeyDown (InputKey::S));
		
	const char* sceneSavePath = dialog2.saveFileDialog (saveScene2, "Assets/Scenes/", ".scene");

	if (strlen (sceneSavePath) > 0) {

		std::string scenePath = FileSystem::Relative (sceneSavePath, fs::current_path ().string ());
		SceneSaver::Instance ().Save (SceneManager::Instance ()->Current (), scenePath);
		SettingsManager::Instance ()->SetValue<std::string> ("Scene", "scene_path", scenePath);
	}
}
