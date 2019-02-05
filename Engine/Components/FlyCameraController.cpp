#include "FlyCameraController.h"

#include <glm/vec3.hpp>
#include <glm/detail/func_geometric.hpp>

#include "Systems/Camera/Camera.h"
#include "Systems/Input/Input.h"
#include "Systems/Time/Time.h"

#include "Editor/Editor.h"

#include "Resources/Resources.h"

#include "Utils/Extensions/MathExtend.h"

void FlyCameraController::Start ()
{
	_pitch = _yaw = 0;

	// Camera::Main ()->SetPosition (glm::vec3 (1200, 1300, 300));		
}

void FlyCameraController::Update ()
{
	if (Editor::WantCaptureInput ()) {
		return;
	}

	float cameraVelocity = 10.0f;
	glm::vec3 velocity = glm::vec3 (0.0f);

	glm::vec3 Forward = Camera::Main ()->GetForward ();
	glm::vec3 Left = Camera::Main ()->GetLeft ();
	Forward = glm::normalize (Forward); Left = glm::normalize (Left);

	if (Input::GetKey (InputKey::W) || Input::GetKey (InputKey::UP) || Input::GetJoystickAxis (0).y < 0) {
		velocity += Forward * cameraVelocity * Time::GetDeltaTime();
	}

	if (Input::GetKey (InputKey::S) || Input::GetKey (InputKey::DOWN) || Input::GetJoystickAxis (0).y > 0) {
		velocity -= Forward * cameraVelocity * Time::GetDeltaTime();
	}

	if (Input::GetKey (InputKey::A) || Input::GetKey (InputKey::LEFT) || Input::GetJoystickAxis (0).x < 0) {
		velocity += Left * cameraVelocity * Time::GetDeltaTime();
	}

	if (Input::GetKey (InputKey::D) || Input::GetKey (InputKey::RIGHT) || Input::GetJoystickAxis (0).x > 0) {
		velocity -= Left * cameraVelocity * Time::GetDeltaTime();
	}

	glm::vec3 camPos = Camera::Main ()->GetPosition () + velocity;
	Camera::Main ()->SetPosition (camPos);

	static bool leftButtonDown = false;
	static int iLastMouseMoveX = 0;
	static int iLastMouseMoveY = 0;

	glm::ivec2 mousePosition = Input::GetMousePosition ();

	if (Input::GetMouseButtonUp (MOUSE_BUTTON_RIGHT)) {
		leftButtonDown = false;
	}
	else if (Input::GetMouseButtonDown (MOUSE_BUTTON_RIGHT)) {
		iLastMouseMoveX = mousePosition.x;
		iLastMouseMoveY = mousePosition.y;

		leftButtonDown = true;
	}

	if (leftButtonDown) {
		int iMoveDistanceX = mousePosition.x - iLastMouseMoveX;
		int iMoveDistanceY = mousePosition.y - iLastMouseMoveY;

		float mouseSensitivity = 0.005f;

		_pitch += iMoveDistanceX * mouseSensitivity;
		_yaw += iMoveDistanceY * mouseSensitivity;

		Camera::Main ()->SetRotation (glm::quat ());
		Camera::Main ()->Rotate (_pitch, glm::vec3 (0.0f, 1.0f, 0.0f));
		Camera::Main ()->Rotate (_yaw, glm::vec3(1.0f, 0.0f, 0.0f));

		iLastMouseMoveX = mousePosition.x;
		iLastMouseMoveY = mousePosition.y;
	}

	if (Input::GetJoystickButton (0) || Input::GetJoystickButton (1) || 
		Input::GetJoystickButton (2) || Input::GetJoystickButton (3)) {

		int iMoveDistanceX = (int)Input::GetJoystickButton (1) + -(int)Input::GetJoystickButton (2);
		int iMoveDistanceY = (int)Input::GetJoystickButton (0) + -(int)Input::GetJoystickButton (3);

		float cameraRotationVelocity = 2.0f;

		_pitch += Time::GetDeltaTime () * iMoveDistanceX * cameraRotationVelocity;
		_yaw += Time::GetDeltaTime () * iMoveDistanceY * cameraRotationVelocity;

		Camera::Main ()->SetRotation (glm::quat ());
		Camera::Main ()->Rotate (_pitch, glm::vec3 (0.0f, 1.0f, 0.0f));
		Camera::Main ()->Rotate (_yaw, glm::vec3(1.0f, 0.0f, 0.0f));
	}
}