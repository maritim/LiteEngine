#ifndef EDITORMAINMENU_H
#define EDITORMAINMENU_H

#include "EditorWidget.h"
#include "EditorManager.h"

#include "Systems/GUI/imguifilesystem/imguifilesystem.h"

class EditorMainMenu : public EditorWidget
{
protected:
	ImGuiFs::Dialog _dialog;

public:
	void Show ();
protected:
	void ShowMainMenu ();
	void CheckInput ();
};

REGISTER_EDITOR_WIDGET(EditorMainMenu)

#endif