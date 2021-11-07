#include "EditorConsole.h"

#include <cstring>

#include "Core/Console/Console.h"

#include "Systems/Settings/SettingsManager.h"

#include "Utils/Extensions/StringExtend.h"

EditorConsole::EditorConsole () :
	_editorConsoleSink (nullptr),
	_selectedIndex (0)
{

}

void EditorConsole::Show ()
{
	bool isConsoleVisible = SettingsManager::Instance ()->GetValue<bool> ("Menu", "show_console", false);

	if (isConsoleVisible == false) {
		return;
	}

	_showLogsMask = SettingsManager::Instance ()->GetValue<int> ("Menu", "console_logs_mask", 30);

	if (_editorConsoleSink == nullptr) {
		_editorConsoleSink = new EditorConsoleSink ();

		Console::AttachSink (_editorConsoleSink);
	}

	ShowConsoleWindow ();
}

void EditorConsole::ShowConsoleWindow ()
{
	if (!ImGui::Begin("Console"))
	{
	    ImGui::End();
	    return;
	}

	std::size_t height = ImGui::GetWindowHeight ();

	ShowConsoleSettings ();

	ImGui::Separator ();

    ImGui::BeginChild("OuterRegion", ImVec2(0, height - 125));

	ShowConsoleLogs ();

    ImGui::EndChild();

    ImGui::Separator ();

    ImGui::BeginChild("OuterRegion2", ImVec2 (0, 25));

	ShowConsoleLog ();

    ImGui::EndChild();

	ImGui::End();
}

void EditorConsole::ShowConsoleSettings ()
{
	if (ImGui::BeginPopup("Options")) {

		bool lastShowDebug = (_showLogsMask & (1 << LogPriority::LOG_DEBUG));
		bool showDebug = lastShowDebug;
		ImGui::Checkbox ("Debug", &showDebug);

		bool lastShowInfo = (_showLogsMask & (1 << LogPriority::LOG_INFO));
		bool showInfo = lastShowInfo;
		ImGui::Checkbox ("Info", &showInfo);

		bool lastShowWarning = (_showLogsMask & (1 << LogPriority::LOG_WARNING));
		bool showWarning = lastShowWarning;
		ImGui::Checkbox ("Warning", &showWarning);

		bool lastShowError = (_showLogsMask & (1 << LogPriority::LOG_ERROR));
		bool showError = lastShowError;
		ImGui::Checkbox ("Error", &showError);

		if (showDebug != lastShowDebug) {
			_showLogsMask ^= (1 << LogPriority::LOG_DEBUG);
		}

		if (showInfo != lastShowInfo) {
			_showLogsMask ^= (1 << LogPriority::LOG_INFO);
		}

		if (showWarning != lastShowWarning) {
			_showLogsMask ^= (1 << LogPriority::LOG_WARNING);
		}

		if (showError != lastShowError) {
			_showLogsMask ^= (1 << LogPriority::LOG_ERROR);
		}

		SettingsManager::Instance ()->SetValue<int> ("Menu", "console_logs_mask", _showLogsMask);

		ImGui::EndPopup();
	}

	if (ImGui::Button("Options")) {
		ImGui::OpenPopup("Options");
	}
	ImGui::SameLine();
	bool clear = ImGui::Button("Clear");
	ImGui::SameLine();
	_filter.Draw("Filter", -100.0f);

	if (clear == true) {
		_editorConsoleSink->ClearLog ();
		_selectedIndex = 0;
	}
}

void EditorConsole::ShowConsoleLogs ()
{
	auto& consoleLogs = _editorConsoleSink->GetLog ();

	for (std::size_t index = 0; index < consoleLogs.size (); index ++) {

		if ((_showLogsMask & (1 << consoleLogs [index].Priority)) == 0) {
			continue;
		}

		if (!_filter.PassFilter (consoleLogs [index].Message.c_str ())) {
			continue;
		}

		if (consoleLogs [index].Priority == LogPriority::LOG_ERROR) {
			ImGui::PushStyleColor (ImGuiCol_Text, ImVec4 (1, 0, 0, 1));
		}

		if (consoleLogs [index].Priority == LogPriority::LOG_WARNING) {
			ImGui::PushStyleColor (ImGuiCol_Text, ImVec4 (1, 1, 0, 1));
		}

		if (consoleLogs [index].Priority == LogPriority::LOG_DEBUG) {
			ImGui::PushStyleColor (ImGuiCol_Text, ImVec4 (0.5, 0, 0.5, 1));
		}

		bool selected = _selectedIndex == index;

		ImGui::PushID (index);
		ImGui::Selectable (consoleLogs [index].Message.c_str (), &selected);
		ImGui::PopID ();

		if (selected == true) {
			_selectedIndex = index;
		}

		if (consoleLogs [index].Priority == LogPriority::LOG_ERROR ||
			consoleLogs [index].Priority == LogPriority::LOG_WARNING ||
			consoleLogs [index].Priority == LogPriority::LOG_DEBUG) {
			ImGui::PopStyleColor ();
		}

		if (ImGui::GetScrollY () >= ImGui::GetScrollMaxY ()) {
			ImGui::SetScrollHereY (1.0f);
		}
	}
}

void EditorConsole::ShowConsoleLog ()
{
	auto& consoleLogs = _editorConsoleSink->GetLog ();

	if (consoleLogs.size () == 0) {
		return;
	}

	std::string message = consoleLogs [_selectedIndex].Message;
	Extensions::StringExtend::Trim (message);

	char text [1000];

	std::memcpy (text, message.c_str (), message.size ());
	text [message.size ()] = '\0';

	std::size_t width = ImGui::GetWindowWidth ();

	ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4 (0, 0, 0, 0));
	ImGui::InputTextMultiline ("", text, message.size (), ImVec2(width, 100), ImGuiInputTextFlags_ReadOnly);
	ImGui::PopStyleColor ();
}
