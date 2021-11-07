#include "EditorHierarchy.h"

#include <ImGui/imgui.h>

#include "EditorSelection.h"

#include "Systems/Settings/SettingsManager.h"

#include "Managers/SceneManager.h"

void EditorHierarchy::Show ()
{
	bool isHierarchyVisible = SettingsManager::Instance ()->GetValue<bool> ("Menu", "show_hierarchy", false);

	if (isHierarchyVisible == true) {
		ShowHierarchy ();
	}
}

void EditorHierarchy::ShowHierarchy ()
{
	if (ImGui::Begin("Hierarchy", NULL)) {

		Scene* scene = SceneManager::Instance ()->Current ();

		bool isItemClicked = false;
		ShowHierarchy (scene->GetRoot (), scene, &isItemClicked);

		if (isItemClicked == false) {
			if (ImGui::IsMouseClicked (ImGuiMouseButton_Left) && ImGui::IsWindowHovered ()) {
				EditorSelection::Instance ()->SetActive (nullptr);
			}
		}
	}

	ImGui::End();
}

void EditorHierarchy::ShowHierarchy (SceneObject* sceneObject, Scene* scene, bool* isItemClicked)
{
	SceneObject* focusedObject = EditorSelection::Instance ()->GetActive ();

	ImGuiTreeNodeFlags node_flags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_OpenOnArrow;

	if (focusedObject == sceneObject) {
		node_flags |= ImGuiTreeNodeFlags_Selected;
	}

	if (sceneObject->GetTransform ()->GetChildrenCount () == 0) {
		node_flags |= ImGuiTreeNodeFlags_Leaf;
	}

	auto col = ImGui::GetStyleColorVec4 (ImGuiCol_Text);

	if (sceneObject->GetTransform ()->GetParent () != nullptr &&
		sceneObject->GetTransform ()->GetParent ()->GetSceneObject ()->IsActive () == false) {
		col = ImVec4 (col.x * 2, col.y * 2, col.z * 2, col.w);
	}

	if (sceneObject->IsActive () == false) {
		col = ImVec4 (col.x / 2, col.y / 2, col.z / 2, col.w);
	}

	ImGui::PushStyleColor (ImGuiCol_Text, col);

	ImGui::PushID (sceneObject);
	bool open = ImGui::TreeNodeEx (sceneObject->GetName ().c_str (), node_flags);

    if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None))
    {
        ImGui::SetDragDropPayload("DragDrop_SceneObject", &sceneObject, sizeof (intptr_t));    // Set payload to carry the index of our item (could be anything)
        ImGui::Text("%s", sceneObject->GetName ().c_str ());    // Display preview (could be anything, e.g. when dragging an image we could decide to display the filename and a small preview of the image, etc.)
        ImGui::EndDragDropSource();
    }
    if (ImGui::BeginDragDropTarget())
    {
        if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("DragDrop_SceneObject")) {
            SceneObject* source = (SceneObject*) * (intptr_t*) payload->Data;

            /*
             * Check source is not parent to current scene object
            */

            SceneObject* parent = sceneObject;
            while (parent->GetTransform ()->GetParent () != nullptr &&
            		parent->GetTransform ()->GetParent ()->GetSceneObject () != source) {
            	parent = parent->GetTransform ()->GetParent ()->GetSceneObject ();
            }

            if (parent->GetTransform ()->GetParent () == nullptr) {
	            source->GetTransform ()->SetParent (sceneObject->GetTransform ());
            }
        }
        ImGui::EndDragDropTarget();
    }
    if (ImGui::BeginPopupContextItem()) // <-- This is using IsItemHovered()
    {
		if (sceneObject->IsActive () == false) {
			ImGui::PushStyleColor (ImGuiCol_Text, ImVec4 (col.x * 2, col.y * 2, col.z * 2, col.w));
		}
        if (ImGui::MenuItem("Attach Scene Object")) {

        	SceneObject* newSceneObject = new SceneObject ();

        	std::size_t instanceID = 0;

        	for (auto sceneObject : *scene) {
        		instanceID = std::max (instanceID, sceneObject->GetInstanceID ());
        	}

        	newSceneObject->SetName ("SceneObject");
        	newSceneObject->SetInstanceID (instanceID + 1);
        	scene->AttachObject (newSceneObject);
        	newSceneObject->GetTransform ()->SetParent (sceneObject->GetTransform ());
        }
        if (ImGui::MenuItem("Detach Scene Object")) {
        	scene->DetachObject (sceneObject);

        	focusedObject = nullptr;
        	EditorSelection::Instance ()->SetActive (nullptr);
        }
		if (sceneObject->IsActive () == false) {
	        ImGui::PopStyleColor ();
		}
        ImGui::EndPopup();
    }

	ImGui::PopID ();

	if (ImGui::IsItemClicked()) {
		if (focusedObject != sceneObject) {
			*isItemClicked = true;
			EditorSelection::Instance ()->SetActive (sceneObject);
		}
	}

	if (open) {
		for_each_type (Transform*, child, *sceneObject->GetTransform ()) {
			ShowHierarchy (child->GetSceneObject (), scene, isItemClicked);
		}

		ImGui::TreePop ();
	}

	ImGui::PopStyleColor ();
}
