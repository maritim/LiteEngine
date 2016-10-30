#include "CameraController.h"

#include "Core/Math/glm/vec3.hpp"
#include "Core/Math/glm/detail/func_geometric.hpp"

#include "Systems/Camera/Camera.h"
#include "Systems/Input/Input.h"
#include "Systems/Time/Time.h"

void CameraController::Start ()
{
	Camera::Main ()->SetPosition (glm::vec3 (170, 100, 170));

	Camera::Main ()->SetYaw (-2.3f);
	Camera::Main ()->SetPitch (-0.3f);
}

void CameraController::Update ()
{
	// http://www.opengl-tutorial.org/beginners-tutorials/tutorial-3-matrices/
	// https://www.youtube.com/watch?v=zZM2uUkEoFw&index=12&list=PLW3Zl3wyJwWOpdhYedlD-yCB7WQoHf-My
	// http://cboard.cprogramming.com/game-programming/135390-how-properly-move-strafe-yaw-pitch-camera-opengl-glut-using-glulookat.html

	float cameraVelocity = 200.0f;
	glm::vec3 velocity = glm::vec3 (0.0f);

	glm::vec3 Forward = Camera::Main ()->ToVector3();
	glm::vec3 Right = glm::cross (Forward, glm::vec3 (0.0f, 1.0f, 0.0f));
	Forward = glm::normalize (Forward); Right = glm::normalize (Right);

	if (Input::GetKey (InputKey::W) || Input::GetKey (InputKey::UP)) {
		velocity += Forward * cameraVelocity * Time::GetDeltaTime();
	}

	if (Input::GetKey (InputKey::S) || Input::GetKey (InputKey::DOWN)) {
		velocity -= Forward * cameraVelocity * Time::GetDeltaTime();
	}

	if (Input::GetKey (InputKey::A) || Input::GetKey (InputKey::LEFT)) {
		velocity -= Right * cameraVelocity * Time::GetDeltaTime();
	}

	if (Input::GetKey (InputKey::D) || Input::GetKey (InputKey::RIGHT)) {
		velocity += Right * cameraVelocity * Time::GetDeltaTime();
	}

	glm::vec3 camPos = Camera::Main ()->GetPosition () + velocity;
	Camera::Main ()->SetPosition (camPos);

	static bool leftButtonDown = false;
	static int iLastMouseMoveX = 0;
	static int iLastMouseMoveY = 0;

	glm::vec3 mousePosition = Input::GetMousePosition ();

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