#include "EditorHierarchy.h"

#include "Systems/GUI/ImGui/imgui.h"

#include "EditorSelection.h"

#include "Managers/SceneManager.h"

#include "Systems/Settings/SettingsManager.h"

void EditorHierarchy::Show ()
{
	bool isHierarchyVisible = SettingsManager::Instance ()->GetValue<bool> ("menu_show_hierarchy", false);

	if (isHierarchyVisible == true) {
		ShowHierarchy ();
	}
}

void EditorHierarchy::ShowHierarchy ()
{
	SceneObject* focusedObject = EditorSelection::Instance ()->GetActive ();

	if (ImGui::Begin("Hierarchy", NULL)) {

		Scene* scene = SceneManager::Instance ()->Current ();

		for (std::size_t objectIndex = 0; objectIndex < scene->GetObjectsCount (); objectIndex ++) {

			SceneObject* object = scene->GetObject (objectIndex);

			ImGuiTreeNodeFlags node_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_Leaf;

			if (focusedObject == object) {
				node_flags |= ImGuiTreeNodeFlags_Selected;
			}

			if (object->IsActive () == false) {
				auto col = ImGui::GetStyleColorVec4 (ImGuiCol_Text);
				col = ImVec4 (col.x / 2, col.y / 2, col.z / 2, col.w);
				ImGui::PushStyleColor (ImGuiCol_Text, col);
			}

			if (ImGui::TreeNodeEx (object->GetName ().c_str (), node_flags)) {

				if (ImGui::IsItemClicked()) {
					if (focusedObject != object) {
						EditorSelection::Instance ()->SetActive (object);
					}
				}

				ImGui::TreePop ();
			}

			if (object->IsActive () == false) {
				ImGui::PopStyleColor ();
			}
		}
	}

	ImGui::End();
}
