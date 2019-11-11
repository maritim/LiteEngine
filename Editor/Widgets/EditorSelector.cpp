#include "EditorSelector.h"

#include <glm/vec3.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Systems/GUI/ImGuizmo/ImGuizmo.h"

#include "SceneNodes/GameObject.h"

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

	if (focusedObject == nullptr) {
		return;
	}

	if (scene->GetObject (focusedObject->GetName ()) == nullptr) {
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

		// if (sceneObject->GetRenderer ()->GetStageType () != Renderer::StageType::DEFERRED_STAGE) {
		// 	continue;
		// }

		/*
		 * Check if it's active
		*/

		if (sceneObject->IsActive () == false) {
			continue;
		}

		/*
		 * Ignore objects without collider
		*/

		GameObject* gameObject = dynamic_cast<GameObject*> (sceneObject);

		if (gameObject == nullptr) {
			continue;
		}

		float distance;

		/*
		 * Check AABB intersection
		*/

		GeometricPrimitive* primitive = gameObject->GetCollider ()->GetGeometricPrimitive ();
		if (!Intersection::Instance ()->CheckRayVsPrimitive (&ray, primitive, distance)) {
			continue;
		}

		/*
		 * Compute model space ray primitive
		*/

		Resource<Model> model = gameObject->GetMesh ();
		Transform* transform = sceneObject->GetTransform ();

		glm::mat4 modelMatrix = transform->GetModelMatrix ();
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
