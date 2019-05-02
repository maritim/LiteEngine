#ifndef EDITORPROJECT_H
#define EDITORPROJECT_H

#include "EditorWidget.h"
#include "EditorManager.h"

class EditorProject : public EditorWidget
{
protected:
	std::string _selectedPath;

public:
	EditorProject ();

	void Show ();
protected:
	void ShowProject ();

	void ShowFolders (const std::string& path);
	void ShowFiles (const std::string& path);

	void Open (const std::string& path);
};

REGISTER_EDITOR_WIDGET(EditorProject)

#endif