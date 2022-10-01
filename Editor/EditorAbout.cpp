#include "EditorAbout.h"

#include <ImGui/imgui.h>
#include <ImGui/imgui_stdlib.h>

#include "EditorSelection.h"

#include "Systems/Settings/SettingsManager.h"
#include "Systems/Components/EditorComponent.h"

#include "Utils/Extensions/MathExtend.h"

void EditorAbout::Show()
{
	bool isInspectorVisible = SettingsManager::Instance()->GetValue<bool>("Menu", "show_about", false);

	if (isInspectorVisible == false) {
		return;
	}

	ShowAbout(EditorSelection::Instance()->GetActive());
}

void EditorAbout::ShowAbout(SceneObject* object)
{
	if (ImGui::Begin("About", NULL)) {

		ImGui::Text("Insider engine 0.6.4_build_1");
		ImGui::Separator();
		ImGui::Text("Forked from liteengine repo by maritim");
		ImGui::Text("Programmed by Almartdev");
	}

	ImGui::End();
}


