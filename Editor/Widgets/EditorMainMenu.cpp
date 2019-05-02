#include "EditorMainMenu.h"

#include "Systems/GUI/ImGui/imgui.h"
#include "imguifilesystem/imguifilesystem.h"

#include "Managers/SceneManager.h"

#include "Systems/Settings/SettingsManager.h"

void EditorMainMenu::Show ()
{
	ShowMainMenu ();
}

void EditorMainMenu::ShowMainMenu ()
{
	if (ImGui::BeginMainMenuBar())
	{
		bool openScene = false;

		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("New")) {}
			
			openScene = ImGui::MenuItem("Open", "Ctrl+O");

			if (ImGui::MenuItem("Save", "Ctrl+S")) {}
			if (ImGui::MenuItem("Save As..")) {}

			ImGui::Separator();

			ImGui::EndMenu();
		}

		static ImGuiFs::Dialog dialog;
		const char* path = dialog.chooseFileDialog (openScene, nullptr, ".scene");

		if (strlen (path) > 0) {
			SceneManager::Instance ()->Load (std::string (path));
		}

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

		if (ImGui::BeginMenu("Windows")) {

			bool lastShowHierarchy = SettingsManager::Instance ()->GetValue<bool> ("menu_show_hierarchy", false);
			bool showHierarchy = lastShowHierarchy;
			ImGui::MenuItem("Hierarchy Window", "CTRL+1", &showHierarchy);

			bool lastShowInspector = SettingsManager::Instance ()->GetValue<bool> ("menu_show_inspector", false);
			bool showInspector = lastShowInspector;
			ImGui::MenuItem ("Inspector Window", "CTRL+2", &showInspector);

			ImGui::Separator ();

			bool lastShowAnimationSettings = SettingsManager::Instance ()->GetValue<bool> ("menu_show_animation_settings", false);
			bool showAnimationSettings = lastShowAnimationSettings;
			ImGui::MenuItem("Animation Settings", "CTRL+3", &showAnimationSettings);

			ImGui::Separator ();

			bool lastShowRenderingSettings = SettingsManager::Instance ()->GetValue<bool> ("menu_show_rendering_settings", false);
			bool showRenderingSettings = lastShowRenderingSettings;
			ImGui::MenuItem("Rendering Settings", "CTRL+5", &showRenderingSettings);

			if (showHierarchy != lastShowHierarchy) {
				SettingsManager::Instance ()->SetValue ("menu_show_hierarchy", std::to_string (showHierarchy));
			}

			if (showInspector != lastShowInspector) {
				SettingsManager::Instance ()->SetValue ("menu_show_inspector", std::to_string (showInspector));
			}

			if (showAnimationSettings != lastShowAnimationSettings) {
				SettingsManager::Instance ()->SetValue ("menu_show_animation_settings", std::to_string (showAnimationSettings));
			}

			if (showRenderingSettings != lastShowRenderingSettings) {
				SettingsManager::Instance ()->SetValue ("menu_show_rendering_settings", std::to_string (showRenderingSettings));
			}

			ImGui::EndMenu();
		}

		ImGui::EndMainMenuBar();
	}
}
