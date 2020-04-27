#ifndef EDITORINSPECTOR_H
#define EDITORINSPECTOR_H

#include "EditorWidget.h"
#include "EditorManager.h"

#include "SceneGraph/SceneObject.h"

class EditorInspector : public EditorWidget
{
public:
	void Show ();
protected:
	void ShowInspector (SceneObject* object);

	void ShowSceneObject (SceneObject* object);
	void ShowTransform (Transform* transform);
	void ShowComponents (SceneObject* object);

	void ShowComponent (Component* component);

	void ShowAttachComponent (SceneObject* object);
};

REGISTER_EDITOR_WIDGET(EditorInspector)

#endif