#include "CameraController.h"

#include "Systems/Camera/Camera.h"
#include "Systems/Input/Input.h"
#include "Systems/Time/Time.h"

void CameraController::Update ()
{
	// http://www.opengl-tutorial.org/beginners-tutorials/tutorial-3-matrices/
	// https://www.youtube.com/watch?v=zZM2uUkEoFw&index=12&list=PLW3Zl3wyJwWOpdhYedlD-yCB7WQoHf-My
	// http://cboard.cprogramming.com/game-programming/135390-how-properly-move-strafe-yaw-pitch-camera-opengl-glut-using-glulookat.html

	float cameraVelocity = 10.0f;
	Vector3 velocity = Vector3::Zero;

	Vector3 Forward = Camera::Main ()->ToVector3();
	Vector3 Right = Forward.Cross (Vector3::Up);
	Forward.Normalize (); Right.Normalize ();

	if (Input::GetKey ('w')) {
		velocity += Forward * cameraVelocity * Time::GetDeltaTime();
	}

	if (Input::GetKey('s')) {
		velocity -= Forward * cameraVelocity * Time::GetDeltaTime();
	}

	if (Input::GetKey('a')) {
		velocity -= Right * cameraVelocity * Time::GetDeltaTime();
	}

	if (Input::GetKey('d')) {
		velocity += Right * cameraVelocity * Time::GetDeltaTime();
	}

	Vector3 camPos = Camera::Main ()->GetPosition () + velocity;
	Camera::Main ()->SetPosition (camPos);

	static bool leftButtonDown = false;
	static int iLastMouseMoveX = 0;
	static int iLastMouseMoveY = 0;

	Vector3 mousePosition = Input::GetMousePosition ();

	if (Input::GetMouseButtonUp (SDL_BUTTON_LEFT)) {
		leftButtonDown = false;
	}
	else if (Input::GetMouseButtonDown (SDL_BUTTON_LEFT)) {
		iLastMouseMoveX = mousePosition.x;
		iLastMouseMoveY = mousePosition.y;

		leftButtonDown = true;
	}

	if (leftButtonDown) {
		int iMoveDistanceX = mousePosition.x - iLastMouseMoveX;
		int iMoveDistanceY = mousePosition.y - iLastMouseMoveY;

		float mouseSensitivity = 0.005f;

		float camYaw = Camera::Main ()->GetYaw () + 
			iMoveDistanceX * mouseSensitivity;
		float camPitch = Camera::Main ()->GetPitch () -
			iMoveDistanceY * mouseSensitivity;
		
		Camera::Main ()->SetYaw (camYaw);
		Camera::Main ()->SetPitch (camPitch);

		iLastMouseMoveX = mousePosition.x;
		iLastMouseMoveY = mousePosition.y;
	}
}