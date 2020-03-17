#ifndef EDITORCONSOLE_H
#define EDITORCONSOLE_H

#include "EditorWidget.h"
#include "EditorManager.h"

#include "Systems/GUI/ImGui/imgui.h"

#include "EditorConsoleSink.h"

class EditorConsole : public EditorWidget
{
protected:
	EditorConsoleSink* _editorConsoleSink;
	std::size_t _selectedIndex;
	ImGuiTextFilter _filter;

public:
	EditorConsole ();

	void Show ();
protected:
	void ShowConsoleWindow ();

	void ShowConsoleSettings ();
	void ShowConsoleLogs ();
	void ShowConsoleLog ();
};

REGISTER_EDITOR_WIDGET(EditorConsole)

#endif