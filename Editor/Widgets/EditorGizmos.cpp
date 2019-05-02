#include "EditorGizmos.h"

#include <glm/vec3.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/matrix_decompose.hpp>
#include "Systems/GUI/ImGui/imgui.h"

#include "Systems/Input/Input.h"
#include "Systems/GUI/Gizmo/Gizmo.h"

#include "EditorScene.h"
#include "EditorSelection.h"

EditorGizmos::EditorGizmos () :
	_editMode (false),
	_lastFocusedObject (nullptr),
	_focusedObject (nullptr),
	_currentOperation (ImGuizmo::TRANSLATE),
	_currentMode (ImGuizmo::LOCAL)
{

}

void EditorGizmos::Show ()
{
	_lastFocusedObject = _focusedObject;

	_focusedObject = EditorSelection::Instance ()->GetActive ();

	/*
	 * Update edit mode
	*/

	_editMode = false;

	if (_focusedObject != nullptr) {
		_editMode = true;
	}

	UpdateMode ();

	HideLastCollider ();

	if (_focusedObject != nullptr) {

		Camera* camera = EditorScene::Instance ()->GetCamera ();
		// if (_editMode == true) {
			ShowGizmo (camera, _focusedObject->GetTransform ());
		// }

		ShowBoundingBox ();
		ShowCollider ();
	}

	ShowGrid ();
}

void EditorGizmos::UpdateMode ()
{
	if (EditorScene::Instance ()->IsWindowHovered () == false) {
		return;
	}

	if (Input::GetKeyDown (InputKey::T)) {
		_currentOperation = ImGuizmo::TRANSLATE;
	}

	if (Input::GetKeyDown (InputKey::R)) {
		_currentOperation = ImGuizmo::ROTATE;		
	}

	if (Input::GetKeyDown (InputKey::Y)) {
		_currentOperation = ImGuizmo::SCALE;
	}

	_snapMode = false;
	if (Input::GetKey (InputKey::LCTRL)) {
		_snapMode = true;
	}
}

void EditorGizmos::ShowGizmo (const Camera* camera, Transform* transform)
{
	static float snap[3] = { 1.f, 1.f, 1.f };
	// static float bounds[] = { -0.5f, -0.5f, -0.5f, 0.5f, 0.5f, 0.5f };
	// static float boundsSnap[] = { 0.1f, 0.1f, 0.1f };
	// static bool boundSizing = false;
	// static bool boundSizingSnap = false;

	float* objectMatrix = GetObjectMatrix (transform);

	glm::vec3 cameraPosition = camera->GetPosition ();
	glm::mat4 viewMatrix = glm::mat4_cast (camera->GetRotation ());

	viewMatrix = glm::translate (viewMatrix, cameraPosition * -1.0f);

	glm::mat4 projectionMatrix = camera->GetProjectionMatrix ();

	glm::ivec2 sceneWindowPos = EditorScene::Instance ()->GetWindowPosition ();
	glm::ivec2 sceneWindowSize = EditorScene::Instance ()->GetWindowSize ();

	ImGuizmo::SetRect(sceneWindowPos.x, sceneWindowPos.y, sceneWindowSize.x, sceneWindowSize.y);

	ImGuizmo::Manipulate(glm::value_ptr (viewMatrix), glm::value_ptr (projectionMatrix),
		_currentOperation, _currentMode, objectMatrix, nullptr,
		_snapMode == true ? snap : nullptr,
		NULL, NULL);

	glm::mat4 result = glm::make_mat4 (objectMatrix);

	glm::vec3 position;
	glm::quat rotation;
	glm::vec3 scale;
	glm::vec3 skew;
	glm::vec4 perspective;

	glm::decompose (result, scale, rotation, position, skew, perspective);

	transform->SetPosition (position);
	transform->SetRotation (rotation);
	transform->SetScale (scale);
}

void EditorGizmos::ShowBoundingBox ()
{
	Collider* collider = _focusedObject->GetCollider ();

	if (collider == nullptr) {
		return;
	}

	auto aabb = (AABBVolume*) collider->GetGeometricPrimitive ();
	auto aabbData = aabb->GetVolumeInformation ();

	glm::vec3 sub = aabbData->maxVertex - aabbData->minVertex;

	Gizmo::DrawLine (aabbData->minVertex, aabbData->minVertex + glm::vec3 (sub.x, 0, 0) , Color::White);
	Gizmo::DrawLine (aabbData->minVertex, aabbData->minVertex + glm::vec3 (0, sub.y, 0) , Color::White);
	Gizmo::DrawLine (aabbData->minVertex, aabbData->minVertex + glm::vec3 (0, 0, sub.z) , Color::White);
	Gizmo::DrawLine (aabbData->maxVertex, aabbData->maxVertex - glm::vec3 (sub.x, 0, 0) , Color::White);
	Gizmo::DrawLine (aabbData->maxVertex, aabbData->maxVertex - glm::vec3 (0, sub.y, 0) , Color::White);
	Gizmo::DrawLine (aabbData->maxVertex, aabbData->maxVertex - glm::vec3 (0, 0, sub.z) , Color::White);
	Gizmo::DrawLine (aabbData->minVertex + glm::vec3 (sub.x, 0, 0), aabbData->maxVertex - glm::vec3 (0, sub.y, 0) , Color::White);
	Gizmo::DrawLine (aabbData->minVertex + glm::vec3 (sub.x, 0, 0), aabbData->maxVertex - glm::vec3 (0, 0, sub.z) , Color::White);
	Gizmo::DrawLine (aabbData->minVertex + glm::vec3 (0, sub.y, 0), aabbData->maxVertex - glm::vec3 (0, 0, sub.z) , Color::White);
	Gizmo::DrawLine (aabbData->minVertex + glm::vec3 (0, sub.y, 0), aabbData->maxVertex - glm::vec3 (sub.x, 0, 0) , Color::White);
	Gizmo::DrawLine (aabbData->minVertex + glm::vec3 (0, 0, sub.z), aabbData->maxVertex - glm::vec3 (sub.x, 0, 0) , Color::White);
	Gizmo::DrawLine (aabbData->minVertex + glm::vec3 (0, 0, sub.z), aabbData->maxVertex - glm::vec3 (0, sub.y, 0) , Color::White);
}

void EditorGizmos::HideLastCollider ()
{
	if (_lastFocusedObject == _focusedObject) {
		return;
	}

	if (_lastFocusedObject != nullptr) {
		_lastFocusedObject->GetRigidbody ()->Debug (false);
	}
}

void EditorGizmos::ShowCollider ()
{
	if (_lastFocusedObject == _focusedObject) {
		return;
	}

	_focusedObject->GetRigidbody ()->Debug (true);
}

void EditorGizmos::ShowGrid ()
{
	const int gridSize = 128;
	
	glm::vec3 cameraPos = EditorScene::Instance ()->GetCamera ()->GetPosition ();
	int gridGap = std::max (1, (int) std::log (std::abs (cameraPos.y)));

	glm::vec3 gridLimit1 = glm::vec3 (
		(int) cameraPos.x - gridSize - ((int) cameraPos.x) % gridGap, 0,
		(int) cameraPos.z - gridSize - ((int) cameraPos.z) % gridGap);
	glm::vec3 gridLimit2 = glm::vec3 (
		(int) cameraPos.x + gridSize - ((int) cameraPos.x) % gridGap, 0,
		(int) cameraPos.z + gridSize - ((int) cameraPos.z) % gridGap);

	for (int index = gridLimit1.x; index <= gridLimit2.x; index += gridGap) {
		Gizmo::DrawLine (glm::vec3 (index, -1.0f, gridLimit1.z ), glm::vec3 (index, -1.0f, gridLimit2.z), Color::White, true);
	}

	for (int index = gridLimit1.z; index <= gridLimit2.z; index += gridGap) {
		Gizmo::DrawLine (glm::vec3 (gridLimit1.x, -1.0f, index), glm::vec3 (gridLimit2.x, -1.0f, index), Color::White, true);
	}
}

float* EditorGizmos::GetObjectMatrix (const Transform* transform) const
{
	static float objectMatrix [16];

	glm::vec3 position = transform->GetPosition ();
	glm::vec3 scalev = transform->GetScale ();
	glm::quat rotationq = transform->GetRotation ();

	glm::mat4 translate = glm::translate (glm::mat4 (1.f), glm::vec3 (position.x, position.y, position.z));
	glm::mat4 scale = glm::scale (glm::mat4 (1.f), glm::vec3 (scalev.x, scalev.y, scalev.z));

	glm::mat4 rotation = glm::mat4_cast(rotationq);

	glm::mat4 modelMatrix = translate * scale * rotation;

	std::memcpy (objectMatrix, glm::value_ptr (modelMatrix), sizeof (float) * 16);

	return objectMatrix;
}
