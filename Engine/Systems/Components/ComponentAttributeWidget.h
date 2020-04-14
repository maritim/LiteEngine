#ifndef COMPONENTATTRIBUTEWIDGET_H
#define COMPONENTATTRIBUTEWIDGET_H

#include "Core/Interfaces/Object.h"

#include <experimental/filesystem>
#include "Systems/GUI/ImGui/imgui.h"
#include "Systems/GUI/ImGui/imgui_stdlib.h"
#include "Systems/GUI/imguifilesystem/imguifilesystem.h"

namespace fs = std::experimental::filesystem;

class ComponentAttributeWidget : public Object
{
public:
	template<class T>
	static bool Show (T&, const std::string& name) { return false; }
};

template <>
bool ComponentAttributeWidget::Show<std::string> (std::string& s, const std::string& name)
{
	return ImGui::InputText (name.c_str (), &s);
}

template <>
bool ComponentAttributeWidget::Show<int> (int& value, const std::string& name)
{
	return ImGui::InputInt (name.c_str (), &value);
}

template <>
bool ComponentAttributeWidget::Show<float> (float& value, const std::string& name)
{
	return ImGui::InputFloat (name.c_str (), &value);
}

template <>
bool ComponentAttributeWidget::Show<bool> (bool& value, const std::string& name)
{
	return ImGui::Checkbox (name.c_str (), &value);
}

template <>
bool ComponentAttributeWidget::Show<Resource<Model>> (Resource<Model>& model, const std::string& name)
{
	std::string widgetName = name + ": " + model->GetName ();

	ImGui::Text (widgetName.c_str ());

	ImGui::SameLine ();

	bool lastLoadTexture = ImGui::Button ("Load", ImVec2 (48, 18));

	static ImGuiFs::Dialog dialog = ImGuiFs::Dialog ();
	std::string meshPath = dialog.chooseFileDialog(lastLoadTexture, "Assets/", ".obj;.dae;");

	meshPath = FileSystem::Relative (meshPath, fs::current_path ().string ());

	if (meshPath != std::string ()) {

		model = Resources::LoadModel (meshPath);

		return true;
	}

	return false;
}

template <>
bool ComponentAttributeWidget::Show<Resource<AudioClip>> (Resource<AudioClip>& audioClip, const std::string& name)
{
	std::string widgetName = name + ": " + audioClip->GetName ();

	ImGui::Text (widgetName.c_str ());

	ImGui::SameLine ();

	bool lastLoadAudioClip = ImGui::Button ("Load", ImVec2 (48, 18));

	static ImGuiFs::Dialog dialog = ImGuiFs::Dialog ();
	std::string audioClipPath = dialog.chooseFileDialog(lastLoadAudioClip, "Assets/", ".wav");

	audioClipPath = FileSystem::Relative (audioClipPath, fs::current_path ().string ());

	if (audioClipPath != std::string ()) {

		audioClip = Resources::LoadAudioClip (audioClipPath);

		return true;
	}

	return false;
}

template <>
bool ComponentAttributeWidget::Show<BulletCollider*> (BulletCollider*& collider, const std::string& name)
{
	if (ImGui::TreeNodeEx (name.c_str (), ImGuiTreeNodeFlags_DefaultOpen)) {

		if (collider == nullptr) {
			ImGui::Button ("Add collider", ImVec2 (300, 50));
		}

		if (collider != nullptr) {

			static int type = 0;
			const char* types[] = { "Box", "Sphere", "Capsule", "Cylinder", "Mesh" };

			if (dynamic_cast<BoxCollider*> (collider) != nullptr) {
				type = 0;
			}
			else if (dynamic_cast<SphereCollider*> (collider) != nullptr) {
				type = 1;
			}
			else if (dynamic_cast<CapsuleCollider*> (collider) != nullptr) {
				type = 2;
			}
			else if (dynamic_cast<CylinderCollider*> (collider) != nullptr) {
				type = 3;
			}
			else if (dynamic_cast<MeshCollider*> (collider) != nullptr) {
				type = 4;
			}

			ImGui::Combo("Type", &type, types, 5);

			glm::vec3 offsetv = collider->GetOffset ();
			float offset[3] = { offsetv.x, offsetv.y, offsetv.z };
			ImGui::InputFloat3 ("Offset", offset);

			if (offset [0] != offsetv.x || offset [1] != offsetv.y || offset [2] != offsetv.z) {
				collider->SetOffset (glm::vec3 (offset [0], offset [1], offset [2]));
			}
		}

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

	return true;
}

template <>
bool ComponentAttributeWidget::Show<Color> (Color& color, const std::string& name)
{
	glm::vec3 lightColor = color.ToVector3 ();
	float color1[3] = { lightColor.x, lightColor.y, lightColor.z };

	bool isModified = ImGui::ColorEdit3 (name.c_str (), color1);

	color = glm::vec3 (color1 [0], color1 [1], color1 [2]);

	return isModified;
}

#include "Components/LightComponent.h"

template <>
bool ComponentAttributeWidget::Show<LightComponent::ShadowInformation> (LightComponent::ShadowInformation& shadow, const std::string& name)
{
	bool isModified = false;

	if (ImGui::TreeNode (name.c_str ())) {

		std::size_t limit1 = 1, limit2 = 4;
		if (ImGui::SliderScalar ("Cascades Count", ImGuiDataType_U32, &shadow.cascadesCount, &limit1, &limit2)) {
			isModified = true;
		}

		int resolution [2] = { shadow.resolution.x, shadow.resolution.y };
		if (ImGui::InputInt2 ("Resolution", resolution)) {
			isModified = true;
		}
		shadow.resolution = glm::ivec2 (resolution [0], resolution [1]);

		if (ImGui::InputFloat ("Bias", &shadow.bias, 0.0f, 1.0f)) {
			isModified = true;
		}

		ImGui::TreePop();
	}

	return isModified;
}

template <>
bool ComponentAttributeWidget::Show<Resource<Font>> (Resource<Font>& font, const std::string& name)
{
	std::string widgetName = name + ": " + font->GetName ();

	ImGui::Text (widgetName.c_str ());

	ImGui::SameLine ();

	bool lastFont = ImGui::Button ("Load", ImVec2 (48, 18));

	static ImGuiFs::Dialog dialog = ImGuiFs::Dialog ();
	std::string fontPath = dialog.chooseFileDialog(lastFont, "Assets/", ".fnt");

	fontPath = FileSystem::Relative (fontPath, fs::current_path ().string ());

	if (fontPath != std::string ()) {

		font = Resources::LoadBitmapFont (fontPath);

		return true;
	}

	return false;
}

#endif