#ifndef EDITORINSPECTOR_H
#define EDITORINSPECTOR_H

#include "EditorWidget.h"
#include "EditorManager.h"

#include "SceneGraph/SceneObject.h"
#include "SceneNodes/GameObject.h"

class EditorInspector : public EditorWidget
{
public:
	void Show ();
protected:
	void ShowInspector (SceneObject* object);

	void ShowTransform (Transform* transform);
	void ShowObject (SceneObject* object);

	void ShowGameObject (SceneObject* object);
	void ShowLight (SceneObject* object);

	void ShowRenderer (SceneObject* object);
	void ShowRigidbody (Rigidbody* rigidbody);
	void ShowAudioSource (AudioSource* audioSource);
};

REGISTER_EDITOR_WIDGET(EditorInspector)

#endif