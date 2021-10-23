#ifndef COMPONENTATTRIBUTEWIDGET_H
#define COMPONENTATTRIBUTEWIDGET_H

#include "Core/Interfaces/Object.h"

#include <filesystem>
#include "Systems/GUI/ImGui/imgui.h"
#include "Systems/GUI/ImGui/imgui_stdlib.h"
#include "Systems/GUI/imguifilesystem/imguifilesystem.h"

namespace fs = std::filesystem;

class ENGINE_API ComponentAttributeWidget : public Object
{
public:
	template <class T>
	static bool Show (T&, const std::string& name) { return false; }
	template <class T>
	static std::string ShowResource (const T&, const std::string& name, const std::string& extensions);
};

template <class T>
std::string ComponentAttributeWidget::ShowResource (const T& value, const std::string& name, const std::string& extensions)
{
	std::string modelPath = value.GetPath ();

	ImGui::InputText (name.c_str (), &modelPath, ImGuiInputTextFlags_ReadOnly);

	ImGui::SameLine (ImGui::GetWindowWidth () - 52);

	bool lastLoadResource = ImGui::Button ("Load", ImVec2 (48, 18));

	static ImGuiFs::Dialog dialog = ImGuiFs::Dialog ();
	std::string resourcePath = dialog.chooseFileDialog(lastLoadResource, "Assets/", extensions.c_str ());

	resourcePath = FileSystem::Relative (resourcePath, fs::current_path ().string ());

	return resourcePath;
}

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
bool ComponentAttributeWidget::Show<glm::vec3> (glm::vec3& value, const std::string& name)
{
	float value1[3] = { value.x, value.y, value.z };

	bool isModified = ImGui::InputFloat3 (name.c_str (), value1);

	value = glm::vec3 (value1 [0], value1 [1], value1 [2]);

	return isModified;
}

template <>
bool ComponentAttributeWidget::Show<Resource<Model>> (Resource<Model>& model, const std::string& name)
{
	auto path = ComponentAttributeWidget::ShowResource<Resource<Model>> (model, name, ".obj;.dae;");

	if (path != std::string ()) {

		model = Resources::LoadModel (path);

		return true;
	}

	return false;
}

template <>
bool ComponentAttributeWidget::Show<Resource<AudioClip>> (Resource<AudioClip>& audioClip, const std::string& name)
{
	auto path = ComponentAttributeWidget::ShowResource<Resource<AudioClip>> (audioClip, name, ".wav;");

	if (path != std::string ()) {

		audioClip = Resources::LoadAudioClip (path);

		return true;
	}

	return false;
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

#include "Components/Lighting/LightComponent.h"

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
	auto path = ComponentAttributeWidget::ShowResource<Resource<Font>> (font, name, ".fnt;");

	if (path != std::string ()) {

		font = Resources::LoadBitmapFont (path);

		return true;
	}

	return false;
}

#endif