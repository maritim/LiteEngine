#include "EditorManager.h"

#include <ImGui/imgui.h>

EditorManager::EditorManager ()
{

}

EditorManager::~EditorManager ()
{
	for (EditorWidget* widget : _widgets) {
		delete widget;
	}
}

void EditorManager::Update ()
{
	ImGuiViewport* viewport = ImGui::GetMainViewport();
	ImGui::SetNextWindowPos(viewport->Pos);
	ImGui::SetNextWindowSize(viewport->Size);
	ImGui::SetNextWindowViewport(viewport->ID);
	ImGui::SetNextWindowBgAlpha(0.0f);

	ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
	window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
	window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	ImGui::Begin("DockSpace Demo", NULL, window_flags);
	ImGui::PopStyleVar(3);

	ImGuiID dockspace_id = ImGui::GetID("MyDockspace");
	ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_PassthruCentralNode;
	ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);

	for (EditorWidget* widget : _widgets) {
		widget->Show ();
	}

    ImGui::End();
}

void EditorManager::RegisterWidget (EditorWidget* widget)
{
	_widgets.push_back (widget);
}

// bool EditorManager::WantCaptureInput ()
// {
// 	ImGuiIO& io = ImGui::GetIO();

// 	return io.WantCaptureMouse || io.WantCaptureKeyboard;
// }
