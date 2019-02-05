#include "EditorGizmosManager.h"

#include <glm/vec3.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Editor/ImGui/imgui.h"

#include "Systems/Window/Window.h"
#include "Systems/Input/Input.h"
#include "Systems/Time/Time.h"

#include "Lighting/LightsManager.h"

void EditorGizmosManager::Start ()
{
	_editMode = false;

	_focusedObject = nullptr;

	_currentOperation = ImGuizmo::TRANSLATE;
	_currentMode = ImGuizmo::LOCAL;
}

void EditorGizmosManager::Update ()
{
	if (Input::GetMouseButtonDown (MOUSE_BUTTON_LEFT)) {

		// TODO: Generalize this

		_focusedObject = nullptr;

		for_each_type (DirectionalLight*, directionalLight, *LightsManager::Instance ()) {
			if (directionalLight->IsActive () == false) {
				continue;
			}

			_focusedObject = directionalLight;
		}

		/*
		 * Update edit mode
		*/

		_editMode = false;

		if (_focusedObject != nullptr) {
			_editMode = true;
		}
	}

	if (Input::GetMouseButtonDown (MOUSE_BUTTON_RIGHT)) {
		_editMode = false;
	}

	if (_focusedObject == nullptr) {
		return;
	}

	if (_editMode == true) {
		if (Input::GetKeyDown (InputKey::T)) {
			_currentOperation = ImGuizmo::TRANSLATE;
		}

		if (Input::GetKeyDown (InputKey::R)) {
			_currentOperation = ImGuizmo::ROTATE;		
		}

		if (Input::GetKeyDown (InputKey::Y)) {
			_currentOperation = ImGuizmo::SCALE;
		}

		ShowGizmo (Camera::Main (), _focusedObject->GetTransform ());
	}

	ShowTransformWidget (_focusedObject->GetTransform ());
}

void EditorGizmosManager::ShowTransformWidget (Transform* transform)
{
	std::size_t windowWidth = Window::GetWidth ();

	ImGui::SetNextWindowPos(ImVec2 (windowWidth - 300, 115), ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowSize (ImVec2 (200, 100), ImGuiCond_FirstUseEver);

	if (ImGui::Begin("Transform", NULL)) {

		glm::vec3 objectPos = transform->GetPosition ();

		float matrixTranslation[3] { objectPos.x, objectPos.y, objectPos.z };
		float matrixRotation[3] { 0, 0, 0 }, matrixScale[3] { 1, 1, 1 };

		ImGui::InputFloat3("Position", matrixTranslation, 3);
		ImGui::InputFloat3("Rotation", matrixRotation, 3);
		ImGui::InputFloat3("Scale", matrixScale, 3);

		switch (_currentOperation)
		{
			case ImGuizmo::OPERATION::TRANSLATE:
				transform->SetPosition (glm::vec3 (matrixTranslation [0], matrixTranslation [1], matrixTranslation [2]));
				break;
			default:
				break;
		}
	}

	ImGui::End();
}

void EditorGizmosManager::ShowGizmo (const Camera* camera, Transform* transform)
{
	// static float snap[3] = { 1.f, 1.f, 1.f };
	// static float bounds[] = { -0.5f, -0.5f, -0.5f, 0.5f, 0.5f, 0.5f };
	// static float boundsSnap[] = { 0.1f, 0.1f, 0.1f };
	// static bool boundSizing = false;
	// static bool boundSizingSnap = false;

	float* objectMatrix = GetObjectMatrix (transform);

	glm::vec3 cameraPosition = camera->GetPosition ();
	glm::mat4 viewMatrix = glm::mat4_cast (camera->GetRotation ());

	viewMatrix = glm::translate (viewMatrix, cameraPosition * -1.0f);

	glm::mat4 projectionMatrix = camera->GetProjectionMatrix ();

	static float deltaMatrix [16];

	ImGuiIO& io = ImGui::GetIO();
	ImGuizmo::SetRect(0, 0, io.DisplaySize.x, io.DisplaySize.y);

	ImGuizmo::Manipulate(glm::value_ptr (viewMatrix), glm::value_ptr (projectionMatrix), _currentOperation, _currentMode, objectMatrix, deltaMatrix, NULL, NULL, NULL);

	switch (_currentOperation)
	{
		case ImGuizmo::OPERATION::TRANSLATE: {

				glm::vec3 pos = transform->GetPosition ();

				pos += glm::vec3 (deltaMatrix [12], deltaMatrix [13], deltaMatrix [14]);

				transform->SetPosition (pos);
			}
			break;
		default:
			break;
	}
}

float* EditorGizmosManager::GetObjectMatrix (const Transform* transform) const
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
