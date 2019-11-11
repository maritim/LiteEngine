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
	if (ImGui::Begin("Hierarchy", NULL)) {

		Scene* scene = SceneManager::Instance ()->Current ();

		SceneRoot* sceneRoot = scene->GetRoot ();

		for_each_type (Transform*, child, *sceneRoot->GetTransform ()) {
			ShowHierarchy (child->GetSceneObject ());
		}
	}

	ImGui::End();
}

void EditorHierarchy::ShowHierarchy (SceneObject* sceneObject)
{
	SceneObject* focusedObject = EditorSelection::Instance ()->GetActive ();

	ImGuiTreeNodeFlags node_flags = ImGuiTreeNodeFlags_OpenOnArrow;

	if (focusedObject == sceneObject) {
		node_flags |= ImGuiTreeNodeFlags_Selected;
	}

	if (sceneObject->GetTransform ()->GetChildrenCount () == 0) {
		node_flags |= ImGuiTreeNodeFlags_Leaf;
	}

	auto col = ImGui::GetStyleColorVec4 (ImGuiCol_Text);

	if (sceneObject->GetTransform ()->GetParent ()->GetSceneObject ()->IsActive () == false) {
		col = ImVec4 (col.x * 2, col.y * 2, col.z * 2, col.w);
	}

	if (sceneObject->IsActive () == false) {
		col = ImVec4 (col.x / 2, col.y / 2, col.z / 2, col.w);
	}

	ImGui::PushStyleColor (ImGuiCol_Text, col);

	bool open = ImGui::TreeNodeEx (sceneObject->GetName ().c_str (), node_flags);

	if (ImGui::IsItemClicked()) {
		if (focusedObject != sceneObject) {
			EditorSelection::Instance ()->SetActive (sceneObject);
		}
	}

	if (open) {
		for_each_type (Transform*, child, *sceneObject->GetTransform ()) {
			ShowHierarchy (child->GetSceneObject ());
		}

		ImGui::TreePop ();
	}

	ImGui::PopStyleColor ();
}
