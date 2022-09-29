#ifndef EDITORIMPORT_H
#define EDITORIMPORT_H

#include "EditorManager.h"

#include "SceneGraph/Scene.h"
#include "SceneGraph/SceneObject.h"

class EditorImporter : public EditorWidget
{
public:
	void Show();
protected:
	void ShowImportWindow();
	char rep[50];
};

REGISTER_EDITOR_WIDGET(EditorImporter)

#endif