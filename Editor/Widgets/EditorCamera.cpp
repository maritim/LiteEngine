#include "EditorCamera.h"

#include <glm/vec3.hpp>
#include <glm/geometric.hpp>

#include "Systems/Cursor/Cursor.h"
#include "Systems/Input/Input.h"
#include "Systems/Time/Time.h"

#include "EditorScene.h"

#include "Utils/Extensions/MathExtend.h"

EditorCamera::EditorCamera () :
	_pitch (0.0f),
	_yaw (0.0f)
{

}

void EditorCamera::Show ()
{
	static bool rightButtonDown = false;

	if (EditorScene::Instance ()->IsWindowHovered () == false) {
		if (rightButtonDown == true) {

			rightButtonDown = false;
			Cursor::Show ();
		}

		return;
	}

	Camera* camera = EditorScene::Instance ()->GetCamera ();

	float cameraVelocity = 10.0f;
	glm::vec3 velocity = glm::vec3 (0.0f);

	glm::vec3 Forward = camera->GetForward ();
	glm::vec3 Left = camera->GetLeft ();
	Forward = glm::normalize (Forward); Left = glm::normalize (Left);

	if (Input::GetKey (InputKey::W)) {
		velocity += Forward * cameraVelocity * Time::GetDeltaTime();
	}

	if (Input::GetKey (InputKey::S)) {
		velocity -= Forward * cameraVelocity * Time::GetDeltaTime();
	}

	if (Input::GetKey (InputKey::A)) {
		velocity += Left * cameraVelocity * Time::GetDeltaTime();
	}

	if (Input::GetKey (InputKey::D)) {
		velocity -= Left * cameraVelocity * Time::GetDeltaTime();
	}

	glm::vec3 camPos = camera->GetPosition () + velocity;
	camera->SetPosition (camPos);

	static int iLastMouseMoveX = 0;
	static int iLastMouseMoveY = 0;

	glm::ivec2 mousePosition = Input::GetMousePosition ();

	if (Input::GetMouseButtonUp (MOUSE_BUTTON_RIGHT)) {
		rightButtonDown = false;

		Cursor::Show ();
	}
	else if (Input::GetMouseButtonDown (MOUSE_BUTTON_RIGHT)) {
		iLastMouseMoveX = mousePosition.x;
		iLastMouseMoveY = mousePosition.y;

		rightButtonDown = true;

		Cursor::Hide ();
	}

	if (rightButtonDown) {
		int iMoveDistanceX = mousePosition.x - iLastMouseMoveX;
		int iMoveDistanceY = mousePosition.y - iLastMouseMoveY;

		float mouseSensitivity = 0.005f;

		_pitch += iMoveDistanceX * mouseSensitivity;
		_yaw += iMoveDistanceY * mouseSensitivity;

		camera->SetRotation (glm::quat ());
		camera->Rotate (_pitch, glm::vec3 (0.0f, 1.0f, 0.0f));
		camera->Rotate (_yaw, glm::vec3(1.0f, 0.0f, 0.0f));

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

		camera->SetRotation (glm::quat ());
		camera->Rotate (_pitch, glm::vec3 (0.0f, 1.0f, 0.0f));
		camera->Rotate (_yaw, glm::vec3(1.0f, 0.0f, 0.0f));
	}
}