#include "EditorInspector.h"

#include <experimental/filesystem>
#include "Systems/GUI/ImGui/imgui.h"
#include "Systems/GUI/imguifilesystem/imguifilesystem.h"

#include "EditorSelection.h"

#include "Managers/AudioClipManager.h"

#include "Mesh/Model.h"

#include "Resources/Resources.h"

#include "Systems/Settings/SettingsManager.h"

#include "Utils/Extensions/MathExtend.h"

#include "Utils/Files/FileSystem.h"

namespace fs = std::experimental::filesystem;

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

			bool lastIsActive = object->IsActive ();
			bool isActive = lastIsActive;
			ImGui::Checkbox (object->GetName ().c_str (), &isActive);

			if (isActive != lastIsActive) {
				object->SetActive (isActive);
			}

			ImGui::Separator ();

			ShowTransform (object->GetTransform ());
			ShowObject (object);

		}
	}

	ImGui::End();	
}

void EditorInspector::ShowTransform (Transform* transform)
{
	if (ImGui::CollapsingHeader ("Transform", ImGuiTreeNodeFlags_DefaultOpen)) {

		glm::vec3 objectPos = transform->GetPosition ();
		float matrixTranslation [3] { objectPos.x, objectPos.y, objectPos.z };
		ImGui::InputFloat3("Position", matrixTranslation, 3);

		glm::quat objectRot = transform->GetRotation ();
		glm::vec3 eulerRot = glm::eulerAngles (objectRot) * RAD2DEG;

		float matrixRotation [3] { eulerRot.x, eulerRot.y, eulerRot.z };
		ImGui::InputFloat3("Rotation", matrixRotation, 3);

		glm::vec3 objectScale = transform->GetScale ();
		float matrixScale [3] { objectScale.x, objectScale.y, objectScale.z };
		ImGui::InputFloat3("Scale", matrixScale, 3);

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

void EditorInspector::ShowObject (SceneObject* object)
{
	if (dynamic_cast<GameObject*> (object) != nullptr) {
		ShowGameObject (object);
	}
}

void EditorInspector::ShowGameObject (SceneObject* object)
{
	ImGui::Spacing ();

	ShowRenderer (object);

	ImGui::Spacing ();

	ShowRigidbody (object->GetRigidbody ());

	ImGui::Spacing ();

	ShowAudioSource (object->GetAudioSource ());
}

void EditorInspector::ShowRenderer (SceneObject* object)
{
	if (ImGui::CollapsingHeader ("Renderer", ImGuiTreeNodeFlags_DefaultOpen)) {
		GameObject* gameObject = dynamic_cast<GameObject*> (object);

		Resource<Model> model = gameObject->GetMesh ();

		std::string modelPath = model->GetName ();

		ImGui::Text ("Model: %s", modelPath.c_str ());

		ImGui::SameLine ();

		bool lastLoadTexture = ImGui::Button ("Load", ImVec2 (48, 18));

		static ImGuiFs::Dialog dialog = ImGuiFs::Dialog ();
		std::string meshPath = dialog.chooseFileDialog(lastLoadTexture, "Assets/", ".obj;.dae;");

		meshPath = FileSystem::Relative (meshPath, fs::current_path ().string ());

		if (meshPath != std::string ()) {

			Resource<Model> mesh = Resources::LoadModel (meshPath);

			gameObject->AttachMesh (mesh);
		}
	}
}

void EditorInspector::ShowRigidbody (Rigidbody* rigidbody)
{
	if (ImGui::CollapsingHeader ("Rigidbody", ImGuiTreeNodeFlags_DefaultOpen)) {

		float mass = rigidbody->GetMass ();
		ImGui::InputFloat ("Mass", &mass);

		if (rigidbody->GetMass () != mass) {
			rigidbody->SetMass (mass);
		}

		if (ImGui::TreeNodeEx ("Collider", ImGuiTreeNodeFlags_DefaultOpen)) {

			BulletCollider* collider = rigidbody->GetCollider ();

			if (collider == nullptr) {
				ImGui::Button ("Add collider", ImVec2 (300, 50));
			}

			if (collider != nullptr) {

				static int type = 0;
				const char* types[] = { "Box", "Sphere", "Capsule", "Cylinder", "Mesh" };

				// if (dynamic_cast<BoxCollider*> (collider) != nullptr) {
				// 	type = 0;
				// }
				// else if (dynamic_cast<SphereCollider*> (collider) != nullptr) {
				// 	type = 1;
				// }
				// else if (dynamic_cast<CapsuleCollider*> (collider) != nullptr) {
				// 	type = 2;
				// }
				// else if (dynamic_cast<CylinderCollider*> (collider) != nullptr) {
				// 	type = 3;
				// }
				// else if (dynamic_cast<MeshCollider*> (collider) != nullptr) {
				// 	type = 4;
				// }

				ImGui::Combo("Type", &type, types, 5);

				glm::vec3 offsetv = collider->GetOffset ();
				float offset[3] = { offsetv.x, offsetv.y, offsetv.z };
				ImGui::InputFloat3 ("Offset", offset);

				if (offset [0] != offsetv.x || offset [1] != offsetv.y || offset [2] != offsetv.z) {
					collider->SetOffset (glm::vec3 (offset [0], offset [1], offset [2]));
				}				
			}

			// if (dynamic_cast<BoxCollider>)

			// std::string modelPath = model->GetName ();

			// ImGui::Text ("Model: %s", modelPath.c_str ());

			// ImGui::SameLine ();

			// bool lastLoadTexture = ImGui::Button ("Load", ImVec2 (48, 18));

			// static ImGuiFs::Dialog dialog = ImGuiFs::Dialog ();
			// std::string meshPath = dialog.chooseFileDialog(lastLoadTexture, "Assets/", ".obj;.dae;");

			// meshPath = FileSystem::Relative (meshPath, fs::current_path ().string ());

			// if (meshPath != std::string ()) {

			// 	Model* mesh = ModelManager::Instance ()->GetModel (meshPath);

			// 	if (mesh == nullptr) {
			// 		mesh = Resources::LoadModel (meshPath);
			// 		ModelManager::Instance ()->AddModel (mesh);
			// 	}

			// 	object->AttachMesh (mesh);
			// }

			ImGui::TreePop ();
		}
	}
}

void EditorInspector::ShowAudioSource (AudioSource* audioSource)
{
	if (ImGui::CollapsingHeader ("Audio Source", ImGuiTreeNodeFlags_DefaultOpen)) {

		AudioClip* audioClip = audioSource->GetAudioClip ();
		std::string path = audioClip == nullptr ? "" : audioClip->GetName ();

		ImGui::Text ("Audio Clip: %s", path.c_str ());

		ImGui::SameLine ();

		bool lastLoadAudioClip = ImGui::Button ("Load", ImVec2 (48, 18));

		bool isPlaying = audioSource->IsPlaying ();
		ImGui::Checkbox ("IsPlaying", &isPlaying);

		if (isPlaying != audioSource->IsPlaying ()) {
			if (isPlaying == false) {
				audioSource->Stop ();
			}

			if (isPlaying == true) {
				audioSource->Play ();
			}
		}

		static ImGuiFs::Dialog dialog = ImGuiFs::Dialog ();
		std::string audioClipPath = dialog.chooseFileDialog(lastLoadAudioClip, "Assets/", ".wav");

		audioClipPath = FileSystem::Relative (audioClipPath, fs::current_path ().string ());

		if (audioClipPath != std::string ()) {

			AudioClip* audioClip = AudioClipManager::Instance ()->GetAudioClip (audioClipPath);

			if (audioClip == nullptr) {
				audioClip = Resources::LoadAudioClip (audioClipPath);
				AudioClipManager::Instance ()->AddAudioClip (audioClip);
			}

			audioSource->SetAudioClip (audioClip);
		}
	}
}