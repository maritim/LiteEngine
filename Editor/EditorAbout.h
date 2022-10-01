#ifndef EDITORABOUT_H
#define EDITORABOUT_H

#include "Widgets\EditorWidget.h"
#include "EditorManager.h"

#include "SceneGraph/SceneObject.h"

class EditorAbout : public EditorWidget
{
public:
	void Show();
protected:
	void ShowAbout(SceneObject* object);
};

REGISTER_EDITOR_WIDGET(EditorAbout)

#endif