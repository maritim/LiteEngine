#include "EditorGizmosManager.h"

#include <glm/vec3.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Editor/ImGui/imgui.h"

#include "Systems/Window/Window.h"
#include "Systems/Input/Input.h"
#include "Systems/Time/Time.h"

#include "Editor/Editor.h"

#include "Core/Intersections/Intersection.h"

#include "Managers/SceneManager.h"

void EditorGizmosManager::Start ()
{
	_editMode = false;

	_focusedObject = nullptr;

	_currentOperation = ImGuizmo::TRANSLATE;
	_currentMode = ImGuizmo::LOCAL;
}

void EditorGizmosManager::Update ()
{
	UpdateMode ();

	if (_focusedObject == nullptr) {
		return;
	}

	if (_editMode == true) {
		ShowGizmo (Camera::Main (), _focusedObject->GetTransform ());
	}

	ShowTransformWidget (_focusedObject->GetTransform ());
}

void EditorGizmosManager::UpdateMode ()
{
	if (Editor::WantCaptureInput ()) {
		return;
	}

	if (Input::GetMouseButtonDown (MOUSE_BUTTON_LEFT)) {

		/*
		 * Get selected scene object
		*/

		glm::ivec2 mousePosition = Input::GetMousePosition ();

		_focusedObject = GetSelectedObject (mousePosition);

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
	}
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

SceneObject* EditorGizmosManager::GetSelectedObject (const glm::ivec2& pos)
{
	/*
	 * Compute world space ray primitve
	*/

	Camera* camera = Camera::Main ();

	glm::vec3 origin = camera->GetPosition ();

	std::size_t screenWidth = Window::GetWidth ();
	std::size_t screenHeight = Window::GetHeight ();

	glm::vec4 viewport = glm::vec4 (0, 0, screenWidth, screenHeight);
	glm::vec3 wincoord = glm::vec3 (pos.x, screenHeight - pos.y - 1, 1);

	glm::vec3 cameraPosition = camera->GetPosition ();
	glm::mat4 view = glm::mat4_cast (camera->GetRotation ());

	view =  glm::translate (view, cameraPosition * -1.0f);

	glm::mat4 projection = camera->GetProjectionMatrix ();

	glm::vec3 objcoord = glm::unProject (wincoord, view, projection, viewport);

	RayPrimitive ray (origin, objcoord - origin);

	/*
	 * Get selected object
	*/

	SceneObject* selectedObject = nullptr;
	float selectedObjectDistance = std::numeric_limits<float>::infinity ();

	for (SceneObject* sceneObject : *SceneManager::Instance ()->Current ()) {

		/*
		 *
		*/

		if (sceneObject->GetRenderer ()->GetStageType () != Renderer::StageType::DEFERRED_STAGE) {
			continue;
		}

		/*
		 * Ignore objects without collider
		*/

		if (sceneObject->GetCollider () == nullptr) {
			continue;
		}

		float distance;

		/*
		 * Check AABB intersection
		*/

		GeometricPrimitive* primitive = sceneObject->GetCollider ()->GetGeometricPrimitive ();
		if (!Intersection::Instance ()->CheckRayVsPrimitive (&ray, primitive, distance)) {
			continue;
		}

		/*
		 * Compute model space ray primitive
		*/

		Model* model = dynamic_cast<GameObject*> (sceneObject)->GetMesh ();
		Transform* transform = sceneObject->GetTransform ();

		glm::vec3 position = transform->GetPosition ();
		glm::vec3 scalev = transform->GetScale ();
		glm::quat rotationq = transform->GetRotation ();

		glm::mat4 translate = glm::translate (glm::mat4 (1.f), glm::vec3 (position.x, position.y, position.z));
		glm::mat4 scale = glm::scale (glm::mat4 (1.f), glm::vec3 (scalev.x, scalev.y, scalev.z));

		glm::mat4 rotation = glm::mat4_cast (rotationq);

		glm::mat4 modelMatrix = translate * scale * rotation;
		glm::mat4 invModelMatrix = glm::inverse (modelMatrix);

		glm::vec3 modelOrigin = glm::vec3 (invModelMatrix * glm::vec4 (origin, 1.0f));
		glm::vec3 modelObjcoord = glm::vec3 (invModelMatrix * glm::vec4 (objcoord, 1.0f));

		RayPrimitive modelRay (modelOrigin, modelObjcoord - modelOrigin);

		/*
		 * Check model intersection
		*/

		if (!Intersection::Instance ()->CheckRayVsModel (&modelRay, model, distance)) {
			continue;
		}

		/*
		 * Compute world space distance
		*/

		glm::vec3 rayDirection = glm::normalize (modelObjcoord - modelOrigin);
		glm::vec3 modelHitPos = modelOrigin + rayDirection * distance;

		glm::vec3 worldHitPos = glm::vec3 (modelMatrix * glm::vec4 (modelHitPos, 1.0f));

		distance = glm::distance (worldHitPos, origin);

		/*
		 * Keep the closest object
		*/

		if (distance < selectedObjectDistance) {
			selectedObjectDistance = distance;
			selectedObject = sceneObject;
		}
	}

	return selectedObject;
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
