#include "EditorProject.h"

#include <algorithm>
#include <experimental/filesystem>
#include "Systems/GUI/ImGui/imgui.h"

#include "Systems/Settings/SettingsManager.h"

namespace fs = std::experimental::filesystem;

EditorProject::EditorProject () :
	_selectedPath ("Assets")
{

}

void EditorProject::Show ()
{
	ShowProject ();
}

void EditorProject::ShowProject ()
{
    ImGui::SetNextWindowSize(ImVec2(430,450), ImGuiCond_FirstUseEver);
    if (!ImGui::Begin("Project", NULL))
    {
        ImGui::End();
        return;
    }

    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(2,2));

    ImGui::BeginChild("left pane", ImVec2(200, 0), true);

    ShowFolders ("Assets");

    ImGui::EndChild();
    ImGui::SameLine();

    ImGui::BeginGroup();
    ShowFiles (_selectedPath);
    ImGui::EndGroup();

    ImGui::PopStyleVar();
    ImGui::End();
}

void EditorProject::ShowFolders (const std::string& path)
{
	fs::path fspath (path);

	ImGui::PushID(path.c_str ());                      // Use object uid as identifier. Most commonly you could also use the object pointer as a base ID.
	ImGui::AlignTextToFramePadding();  // Text and Tree nodes are less high than regular widgets, here we add vertical spacing to make the tree lines equal high.

	ImGuiTreeNodeFlags node_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick;

	if (_selectedPath == path) {
		node_flags |= ImGuiTreeNodeFlags_Selected;
	}

	bool haveDirectories = false;

	for (const auto& entry : fs::directory_iterator(fspath)) {
		if (fs::is_directory (entry.status ())) {
			haveDirectories = true;
			break;
		}
	}

	if (haveDirectories == false) {
		node_flags |= ImGuiTreeNodeFlags_Leaf;
	}

	bool node_open = ImGui::TreeNodeEx(fspath.filename ().string ().c_str (), node_flags);

	if (ImGui::IsItemClicked()) {
		_selectedPath = path;
	}

	if (node_open) {
		if (fs::exists(fspath) && fs::is_directory(fspath)) {
			for (const auto& entry : fs::directory_iterator(fspath)) {
				if (fs::is_directory(entry.status())) {
					ImGui::PushID (entry.path ().string ().c_str ());
					ShowFolders(entry.path ().string ());
					ImGui::PopID ();
				}
			}
			ImGui::TreePop ();
		}
	}	
	ImGui::PopID();
}

void EditorProject::ShowFiles (const std::string& path)
{
	fs::path fspath (path);

	ImGui::Text ("%s", path.c_str ());

	ImGui::Spacing ();

    ImVec2 size = ImGui::GetContentRegionMax ();

    ImGui::Columns (std::max (1, int (size.x / 150)), "Files", false);

	for (const auto& entry : fs::directory_iterator(fspath)) {
		if (fs::is_directory (entry.status ())) {
			continue;
		}

        ImGui::AlignTextToFramePadding();

        ImGui::BeginGroup();
        // ImGui::ImageButton(thumb.Get(), { itemSize - 20, itemSize - 16 });// , { 0, 1 }, { 1,0 });

		if (ImGui::Button (entry.path ().filename ().string ().c_str (), ImVec2 (100, 100))) {
			Open (entry.path ().string ());
		}

        ImGui::TextWrapped("%ws", entry.path ().filename ().c_str ());

        ImGui::EndGroup();

        ImGui::SetNextItemWidth(-1);

        ImGui::NextColumn();
	}
}

void EditorProject::Open (const std::string& path)
{
	fs::path fspath (path);

	if (fspath.extension ().string () == ".rsettings") {
		SettingsManager::Instance ()->SetValue ("menu_show_rendering_settings", std::to_string (true));
		SettingsManager::Instance ()->SetValue ("menu_show_rendering_settings_path", path);
	}
}
