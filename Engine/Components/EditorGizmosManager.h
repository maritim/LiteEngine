#ifndef EDITORGIZMOSMANAGER_H
#define EDITORGIZMOSMANAGER_H

#include "Systems/Components/Component.h"
#include "Systems/Components/ComponentsFactory.h"

#include "Editor/ImGui/ImGuizmo.h"

#include "SceneGraph/SceneObject.h"

#include "Systems/Camera/Camera.h"

class EditorGizmosManager : public Component
{
protected:
	bool _editMode;
	bool _snapMode;
	bool _boundingMode;

	SceneObject* _focusedObject;

	ImGuizmo::OPERATION _currentOperation;
	ImGuizmo::MODE _currentMode;

public:
	void Start ();

	void Update ();
protected:
	void UpdateMode ();

	void ShowTransformWidget (Transform* transform);
	void ShowGizmo (const Camera* camera, Transform* transform);

	float* GetObjectMatrix (const Transform* transform) const;

	SceneObject* GetSelectedObject (const glm::ivec2& pos);
};

REGISTER_COMPONENT(EditorGizmosManager)

#endif