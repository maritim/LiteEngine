#ifndef EDITORCONSOLE_H
#define EDITORCONSOLE_H

#include "EditorWidget.h"
#include "EditorManager.h"

class EditorConsole : public EditorWidget
{
public:
	void Show ();
};

REGISTER_EDITOR_WIDGET(EditorConsole)

#endif