#ifndef EDITORMAINMENU_H
#define EDITORMAINMENU_H

#include "EditorWidget.h"
#include "EditorManager.h"

class EditorMainMenu : public EditorWidget
{
public:
	void Show ();
protected:
	void ShowMainMenu ();
};

REGISTER_EDITOR_WIDGET(EditorMainMenu)

#endif