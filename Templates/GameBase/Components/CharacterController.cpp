#include "CharacterController.h"

#include <glm/vec3.hpp>
#include <glm/geometric.hpp>
#include <glm/gtx/quaternion.hpp>

#include "Systems/Camera/Camera.h"
#include "Systems/Input/Input.h"
#include "Systems/Time/Time.h"
#include "Systems/Physics/PhysicsManager.h"

#include "Managers/SceneManager.h"

#include "Resources/Resources.h"

#include "Utils/Extensions/MathExtend.h"

void CharacterController::Start ()
{
	auto object = SceneManager::Instance ()->Current ()->GetObject ("Eve");
	_object = dynamic_cast<AnimationGameObject*> (object);

	_walkSpeed = 2.5f;
	_walkBackSpeed = 1.5f;
	_runSpeed = 5.0f;

	_rotationSpeed = 2.0f;

	_accelerationSpeed = 2.0f;

	_needJump = false;

	_needTurnLeft = false; _needTurnRight = false;

	_inJump = false;

	_isIdle = true; _isWalking = false;
	_isWalkingBack = false; _isRunning = false;

	if (_object != nullptr) {
		_object->SetAnimationClip ("Idle");
	}
}

void CharacterController::Update ()
{
	if (_object == nullptr) {
		return;
	}

	MovePlayer ();
	AnimatePlayer ();
}

#include "Debug/Logger/Logger.h"

void CharacterController::MovePlayer ()
{
	Transform* transform = _object->GetTransform ();
	// Rigidbody* rigidBody = _object->GetRigidbody ();

	_velocity = glm::vec3 (0.0f);
	_currentSpeed = _walkSpeed;

	glm::quat rotation = glm::quat (1.0f, 0.0f, 0.0f, 0.0f);

	glm::vec3 forward = transform->GetRotation () * glm::vec3 (0.0f, 0.0f, 1.0f);
	glm::vec3 left = transform->GetRotation () * glm::vec3 (-1.0f, 0.0f, 0.0f);
	forward = glm::normalize (forward); left = glm::normalize (left);

	if (Input::GetKey (InputKey::LSHIFT) || Input::GetJoystickButton (3)) {
		_currentSpeed = _runSpeed;
	}

	if (Input::GetKey (InputKey::SPACE) || Input::GetJoystickButton (0)) {
		if (_inJump == false) {
			_needJump = true;
		}
	}

	if (Input::GetKey (InputKey::W) || Input::GetJoystickAxis (0).y < 0) {
		_speed = std::max (_currentSpeed, _accelerationSpeed * Time::GetDeltaTime ());
		_velocity += _speed * forward * Time::GetDeltaTime ();
	}

	if (Input::GetKey (InputKey::S) || Input::GetJoystickAxis (0).y > 0) {
		_currentSpeed = _walkBackSpeed;
		_speed = std::max (_currentSpeed, _accelerationSpeed * Time::GetDeltaTime ());
		_velocity -= _speed * forward * Time::GetDeltaTime ();
	}

	if (Input::GetKey (InputKey::A) || Input::GetJoystickAxis (0).x < 0) {
		rotation *= glm::angleAxis (_rotationSpeed * Time::GetDeltaTime (), glm::vec3 (0, 1, 0));
	}

	if (Input::GetKey (InputKey::D) || Input::GetJoystickAxis (0).x > 0) {
		rotation *= glm::angleAxis (_rotationSpeed * Time::GetDeltaTime (), glm::vec3 (0, -1, 0));
	}

	glm::vec3 newPlayerPos = transform->GetPosition () + _velocity;
	glm::quat newPlayerRot = transform->GetRotation () * rotation;

	bool isCollision = false;

	for (float checkOffset = (_inJump == true) ? 2.4f : 0.1f; checkOffset < 2.0f; checkOffset += 0.5f) {
		glm::vec3 checkOrigin = transform->GetPosition () + glm::vec3 (0.0f, checkOffset, 0.0f);
		glm::vec3 checkDirection = newPlayerPos - transform->GetPosition ();
		float checkDistance = 0.5f;

		if (PhysicsManager::Instance ()->Raycast (checkOrigin, checkDirection, checkDistance).isCollision == true) {
			isCollision = true;
			break;
		}
	}

	if (isCollision == false) {
		transform->SetPosition (newPlayerPos);
	}

	transform->SetRotation (newPlayerRot);
}

void CharacterController::AnimatePlayer ()
{
	//TODO: Change in state machine

	if (_needJump == true || _inJump == true) {
		if (_needJump == true) {

			if (_isRunning == true || _isWalking == true) {
				_object->Blend ("RunJump", 0.2f);
				_jumpElapsedTime = 1.0f;
			}

			if (_isIdle == true) {
				_object->Blend ("IdleJump", 0.2f);
				_jumpElapsedTime = 2.6f;
			}

			if (_isWalkingBack == true) {
				_object->Blend ("BackJump", 0.2f);
				_jumpElapsedTime = 0.87;
			}

			_needJump = false;
			_inJump = true;
		} else {
			_jumpElapsedTime -= Time::GetDeltaTime ();

			if (_jumpElapsedTime < 0.0f) {
				_inJump = false;

				if (_isRunning == true) {
					_object->Blend ("Run", 0.2f);
				}

				if (_isWalking == true) {
					_object->Blend ("Walk", 0.2f);
				}

				if (_isWalkingBack == true) {
					_object->Blend ("WalkBack", 0.2f);
				}

				if (_isIdle == true) {
					_object->Blend ("Idle", 0.2f);
				}
			}
		}
	} else {
		if (glm::length (_velocity) > 0.0001f && _isIdle == true) {

			if (_currentSpeed == _runSpeed) {
				_object->Blend ("Run", 0.2f);
				_isRunning = true;
			}

			if (_currentSpeed == _walkBackSpeed) {
				_object->Blend ("WalkBack", 0.2f);
				_isWalkingBack = true;
			}

			if (_currentSpeed == _walkSpeed) {
				_object->Blend ("Walk", 0.2f);
				_isWalking = true;
			}

			_isIdle = false;
		}

		if (glm::length (_velocity) > 0.0001f && _isWalking == true) {
			if (_currentSpeed == _runSpeed) {
				_object->Blend ("Run", 0.2f);
				_isWalking = false;
				_isRunning = true;
			}

			if (_currentSpeed == _walkBackSpeed) {
				_object->Blend ("WalkBack", 0.2f);
				_isWalking = false;
				_isWalkingBack = true;
			}
		}

		if (glm::length (_velocity) > 0.0001f && _isRunning == true) {
			if (_currentSpeed == _walkSpeed) {
				_object->Blend ("Walk", 0.2f);
				_isWalking = true;
				_isRunning = false;
			}

			if (_currentSpeed == _walkBackSpeed) {
				_object->Blend ("WalkBack", 0.2f);
				_isWalkingBack = true;
				_isRunning = false;
			}
		}

		if (glm::length (_velocity) > 0.0001f && _isWalkingBack == true) {
			if (_currentSpeed == _walkSpeed) {
				_object->Blend ("Walk", 0.2f);
				_isWalking = true;
				_isWalkingBack = false;
			}

			if (_currentSpeed == _runSpeed) {
				_object->Blend ("Run", 0.2f);
				_isRunning = false;
				_isWalkingBack = true;
			}
		}

		if (glm::length (_velocity) < 0.0001f && (_isWalking == true || _isWalkingBack == true || _isRunning == true)) {
			_object->Blend ("Idle", 0.2f);

			_speed = 0.0f;

			_isIdle = true; _isWalking = false;
			_isWalkingBack = false; _isRunning = false;
		}
	}
}
