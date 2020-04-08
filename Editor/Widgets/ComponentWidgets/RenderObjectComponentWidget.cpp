#include "RenderObjectComponentWidget.h"

#include <experimental/filesystem>
#include "Systems/GUI/ImGui/imgui.h"
#include "Systems/GUI/imguifilesystem/imguifilesystem.h"

#include "Components/RenderObjectComponent.h"

#include "Mesh/Model.h"

#include "Resources/Resources.h"

#include "Utils/Files/FileSystem.h"

namespace fs = std::experimental::filesystem;

void RenderObjectComponentWidget::Show (Component* component)
{
	auto renderObjectComponent = (RenderObjectComponent*) component;

	if (ImGui::CollapsingHeader (component->GetName ().c_str (), ImGuiTreeNodeFlags_DefaultOpen)) {
		Resource<Model> model = renderObjectComponent->GetModel ();

		std::string modelPath = model->GetName ();

		ImGui::Text ("Model: %s", modelPath.c_str ());

		ImGui::SameLine ();

		bool lastLoadTexture = ImGui::Button ("Load", ImVec2 (48, 18));

		static ImGuiFs::Dialog dialog = ImGuiFs::Dialog ();
		std::string meshPath = dialog.chooseFileDialog(lastLoadTexture, "Assets/", ".obj;.dae;");

		meshPath = FileSystem::Relative (meshPath, fs::current_path ().string ());

		if (meshPath != std::string ()) {

			Resource<Model> mesh = Resources::LoadModel (meshPath);

			renderObjectComponent->SetModel (mesh);
		}
	}
}
