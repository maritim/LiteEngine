#ifndef EDITORGIZMOS_H
#define EDITORGIZMOS_H

#include "EditorWidget.h"
#include "EditorManager.h"

#include "ImGuizmo/ImGuizmo.h"

#include "SceneGraph/SceneObject.h"

#include "Systems/Camera/Camera.h"

class EditorGizmos : public EditorWidget
{
protected:
	bool _editMode;
	bool _snapMode;
	bool _boundingMode;

	SceneObject* _lastFocusedObject;
	SceneObject* _focusedObject;

	ImGuizmo::OPERATION _currentOperation;
	ImGuizmo::MODE _currentMode;

public:
	EditorGizmos ();

	void Show ();
protected:
	void UpdateMode ();

	void ShowHierarchy ();

	void ShowGizmo (const Camera* camera, Transform* transform);
	void ShowBoundingBox ();
	void ShowCollider ();

	void HideLastCollider ();

	void ShowGrid ();

	float* GetObjectMatrix (const Transform* transform) const;
};

REGISTER_EDITOR_WIDGET(EditorGizmos)

#endif