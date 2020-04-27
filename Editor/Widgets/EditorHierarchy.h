#ifndef EDITORHIERARCHY_H
#define EDITORHIERARCHY_H

#include "EditorWidget.h"
#include "EditorManager.h"

#include "SceneGraph/Scene.h"
#include "SceneGraph/SceneObject.h"

class EditorHierarchy : public EditorWidget
{
public:
	void Show ();
protected:
	void ShowHierarchy ();

	void ShowHierarchy (SceneObject* sceneObject, Scene* scene);

	void ShowHierarchySettings (Scene* scene);
};

REGISTER_EDITOR_WIDGET(EditorHierarchy)

#endif