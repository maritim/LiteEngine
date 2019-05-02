#ifndef EDITORSELECTOR_H
#define EDITORSELECTOR_H

#include "EditorWidget.h"
#include "EditorManager.h"

#include "SceneGraph/SceneObject.h"

class EditorSelector : public EditorWidget
{
public:
	void Show ();
protected:
	void UpdateFocusedObject ();
	void UpdateMode ();

	SceneObject* GetSelectedObject (const glm::ivec2& pos);
};

REGISTER_EDITOR_WIDGET(EditorSelector)

#endif