#include <ImGui/imgui.h>

#include "EditorPackageImporter.h"
#include "EditorSelection.h"

#include "Systems/Settings/SettingsManager.h"
#include <fstream>
#include "Managers/SceneManager.h"


void EditorImporter::Show()
{
	bool isImporterVisible = SettingsManager::Instance()->GetValue<bool>("Menu", "show_packageimp", false);

	if (isImporterVisible == true) {
		ShowImportWindow();
	}
}

void EditorImporter::ShowImportWindow() 
{
	ImGui::Begin("Package importer");
	ImGui::Text("Import your packages from github or gitlab writing here the HTTPS or SSH.");
	ImGui::Text("The imported package will be downloaded to the assets folder.");
	ImGui::InputText("Package repository", rep, IM_ARRAYSIZE(rep));
	std::ofstream ofs("PackageImporterPath.txt", std::ofstream::trunc);
	ofs << rep;
	ofs.close();

	ImGui::Separator();
	if (ImGui::Button("Import"))
	{
		system("cd ..");
		system("python3 packageimporter.py");
		system("python packageimporter.py");
	}

	ImGui::End();
}


