#ifndef EDITORHIERARCHY_H
#define EDITORHIERARCHY_H

#include "EditorWidget.h"
#include "EditorManager.h"

class EditorHierarchy : public EditorWidget
{
public:
	void Show ();
protected:
	void ShowHierarchy ();
};

REGISTER_EDITOR_WIDGET(EditorHierarchy)

#endif