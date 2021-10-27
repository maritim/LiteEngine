#include "EditorInspector.h"

#include <ImGui/imgui.h>
#include <ImGui/imgui_stdlib.h>

#include "EditorSelection.h"

#include "Systems/Components/EditorComponent.h"

#include "Systems/Settings/SettingsManager.h"

#include "Utils/Extensions/MathExtend.h"

void EditorInspector::Show ()
{
	bool isInspectorVisible = SettingsManager::Instance ()->GetValue<bool> ("menu_show_inspector", false);

	if (isInspectorVisible == false) {
		return;
	}

	ShowInspector (EditorSelection::Instance ()->GetActive ());
}

void EditorInspector::ShowInspector (SceneObject* object)
{
	if (ImGui::Begin("Inspector", NULL)) {

		if (object != nullptr) {

			ShowSceneObject (object);
			ShowTransform (object->GetTransform ());
			ShowComponents (object);

		}
	}

	ImGui::End();	
}

void EditorInspector::ShowSceneObject (SceneObject* sceneObject)
{
	std::string lastSceneObjectName = sceneObject->GetName ();
	std::string sceneObjectName = lastSceneObjectName;
	ImGui::InputText ("Name", &sceneObjectName);

	if (lastSceneObjectName != sceneObjectName) {
		sceneObject->SetName (sceneObjectName);
	}

	ImGui::SameLine ();

	bool lastIsActive = sceneObject->IsActive ();
	bool isActive = lastIsActive;
	ImGui::Checkbox ("Active", &isActive);

	if (isActive != lastIsActive) {
		sceneObject->SetActive (isActive);
	}

	ShowAttachComponent (sceneObject);
	bool attachComponent = ImGui::Button ("Attach Component", ImVec2 (ImGui::GetWindowWidth () - 10, 18));

	if (attachComponent) {
		ImGui::OpenPopup("Options");
	}
}

void EditorInspector::ShowTransform (Transform* transform)
{
	ImGui::Separator ();

	if (ImGui::CollapsingHeader ("Transform", ImGuiTreeNodeFlags_DefaultOpen)) {

		glm::vec3 objectPos = transform->GetPosition ();
		float matrixTranslation [3] { objectPos.x, objectPos.y, objectPos.z };
		ImGui::InputFloat3("Position", matrixTranslation, "%.5f");

		glm::quat objectRot = transform->GetRotation ();
		glm::vec3 eulerRot = glm::eulerAngles (objectRot) * RAD2DEG;

		float matrixRotation [3] { eulerRot.x, eulerRot.y, eulerRot.z };
		ImGui::InputFloat3("Rotation", matrixRotation, "%.5f");

		glm::vec3 objectScale = transform->GetScale ();
		float matrixScale [3] { objectScale.x, objectScale.y, objectScale.z };
		ImGui::InputFloat3("Scale", matrixScale, "%.5f");

		if (objectPos.x != matrixTranslation [0]
			|| objectPos.y != matrixTranslation [1]
			|| objectPos.z != matrixTranslation [2]) {

			transform->SetPosition (glm::vec3 (
				matrixTranslation [0],
				matrixTranslation [1],
				matrixTranslation [2]
			));
		}

		if (eulerRot.x != matrixRotation [0]
			|| eulerRot.y != matrixRotation [1]
			|| eulerRot.z != matrixRotation [2]) {

			glm::quat rot = glm::quat (glm::vec3 (
				matrixRotation [0] * DEG2RAD,
				matrixRotation [1] * DEG2RAD,
				matrixRotation [2] * DEG2RAD
			));

			transform->SetRotation (rot);
		}

		if ((objectScale.x != matrixScale [0]
			|| objectScale.y != matrixScale [1]
			|| objectScale.z != matrixScale [2])
			&& matrixScale [0] != 0 && matrixScale [1] != 0 && matrixScale [2] != 0) {

			transform->SetScale (glm::vec3 (
				matrixScale [0],
				matrixScale [1],
				matrixScale [2]
			));
		}
	}
}

void EditorInspector::ShowComponents (SceneObject* object)
{
	for_each_type (Component*, component, *object) {
		ShowComponent (component);
	}
}

void EditorInspector::ShowComponent (Component* component)
{
	auto editorComponent = dynamic_cast<EditorComponent*> (component);

	if (editorComponent == nullptr) {
		return;
	}

	editorComponent->Show ();
}

void EditorInspector::ShowAttachComponent (SceneObject* sceneObject)
{
	if (ImGui::BeginPopup("Options")) {

		for (auto& component : *ObjectsFactory<Component>::Instance ()) {
			std::string componentName = component.first;

			componentName.erase (0, 2);

			if (ImGui::MenuItem(componentName.c_str ())) {

				componentName = "HT" + componentName;
				Component* component = ObjectsFactory<Component>::Instance ()->Create (componentName);

				sceneObject->AttachComponent (component);

				break;
			}
		}

		ImGui::EndPopup();
	}
}
