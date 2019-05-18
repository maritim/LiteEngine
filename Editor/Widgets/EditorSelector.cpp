#include "EditorSelector.h"

#include <glm/vec3.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Systems/GUI/ImGuizmo/ImGuizmo.h"

#include "EditorScene.h"
#include "EditorSelection.h"

#include "Systems/Input/Input.h"

#include "Managers/SceneManager.h"

#include "Core/Intersections/Intersection.h"

void EditorSelector::Show ()
{
	UpdateFocusedObject ();

	UpdateMode ();
}

void EditorSelector::UpdateMode ()
{
	if (EditorScene::Instance ()->IsWindowHovered () == false || ImGuizmo::IsOver () == true) {
		return;
	}

	if (Input::GetMouseButtonDown (MOUSE_BUTTON_LEFT)) {

		/*
		 * Get selected scene object
		*/

		glm::ivec2 mousePosition = EditorScene::Instance ()->GetWindowMousePosition ();

		SceneObject* focusedObject = GetSelectedObject (mousePosition);

		EditorSelection::Instance ()->SetActive (focusedObject);
	}
}

void EditorSelector::UpdateFocusedObject ()
{
	Scene* scene = SceneManager::Instance ()->Current ();

	SceneObject* focusedObject = EditorSelection::Instance ()->GetActive ();

	bool exists = false;

	for (std::size_t index = 0; index < scene->GetObjectsCount (); index ++) {
		if (focusedObject != scene->GetObject (index)) {
			continue;
		}

		exists = true;
		break;
	}

	if (exists == false) {
		EditorSelection::Instance ()->SetActive (nullptr);
	}
}

SceneObject* EditorSelector::GetSelectedObject (const glm::ivec2& pos)
{
	/*
	 * Compute world space ray primitve
	*/

	Camera* camera = EditorScene::Instance ()->GetCamera ();

	glm::vec3 origin = camera->GetPosition ();

	glm::ivec2 screenSize = EditorScene::Instance ()->GetWindowSize ();

	glm::vec4 viewport = glm::vec4 (0, 0, screenSize.x, screenSize.y);
	glm::vec3 wincoord = glm::vec3 (pos.x, screenSize.y - pos.y, 1);

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
