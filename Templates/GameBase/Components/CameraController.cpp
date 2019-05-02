#include "CameraController.h"

#include <glm/vec3.hpp>
#include <glm/geometric.hpp>
#include <glm/gtx/quaternion.hpp>

#include "Systems/Time/Time.h"

#include "Managers/SceneManager.h"
#include "Managers/CameraManager.h"

#include "Utils/Extensions/MathExtend.h"

void CameraController::Start ()
{
	_object = SceneManager::Instance ()->Current ()->GetObject ("Eve");

	_cameraMoveSpeed = 4.0f;
	_cameraRotSpeed = 2.0f;

	_playerRadius = 3.2f;

	_cameraOffset = glm::vec3 (0.0f, 0.4f, -5.4f);

	if (_object) {
		// Transform* transform = _object->GetTransform ();
		// glm::vec3 playerPos = GetPlayerPosition ();

		// Camera::Main ()->SetPosition (playerPos + transform->GetRotation () * _cameraOffset);
	}
}

void CameraController::Update ()
{
	if (_object == nullptr) {
		return;
	}

	MoveCamera ();

	RotateCamera ();
}

void CameraController::MoveCamera ()
{
	Camera* camera = CameraManager::Instance ()->GetActive ();
	Transform* transform = _object->GetTransform ();

	glm::vec3 playerPos = GetPlayerPosition ();

	glm::vec3 camPos = playerPos + transform->GetRotation () * _cameraOffset;

	glm::vec3 direction = glm::normalize (camPos - playerPos);
	glm::vec3 curDirection = glm::normalize (camera->GetPosition () - playerPos);

	float distance = glm::distance (direction * _playerRadius, curDirection * _playerRadius);

	float t = Extensions::MathExtend::Clamp ((_cameraMoveSpeed * Time::GetDeltaTime ()) / distance, 0.0f, 1.0f);

	glm::vec3 destDirection = glm::normalize (Extensions::MathExtend::Lerp (t, curDirection, direction)) * _playerRadius;

	// camPos = Camera::Main ()->GetPosition () + glm::normalize (direction) * _cameraMoveSpeed * Time::GetDeltaTime ();

	camPos = playerPos + destDirection;

	camera->SetPosition (camPos);
}

void CameraController::RotateCamera ()
{
	Camera* camera = CameraManager::Instance ()->GetActive ();

	glm::vec3 camPos = camera->GetPosition ();

	glm::vec3 playerPos = GetPlayerPosition ();

	glm::quat camRot = glm::toQuat (glm::lookAt (camPos, playerPos, glm::vec3 (0, 1, 0)));

	camera->SetRotation (camRot);
}

glm::vec3 CameraController::GetPlayerPosition ()
{
	Transform* transform = _object->GetTransform ();

	// glm::vec3 left = transform->GetRotation () * glm::vec3 (-1.0f, 0.0f, 0.0f);

	glm::vec3 playerPos = transform->GetPosition () + glm::vec3 (0.0f, 2.4f, 0.0f);

	return playerPos;
}
