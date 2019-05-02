#include "PuzzleController.h"

#include <glm/vec3.hpp>
#include <glm/geometric.hpp>
#include <glm/gtx/quaternion.hpp>

#include "Systems/Time/Time.h"
#include "Systems/Input/Input.h"

#include "Managers/SceneManager.h"

#include "Resources/Resources.h"

#include "Debug/Logger/Logger.h"

void PuzzleController::Start ()
{
	_statuesState.resize (8, true);
	_statueDoorLink.resize (8, -1);

	for (std::size_t statueIndex = 1; statueIndex < 9; statueIndex ++) {
		std::string objectName = "BunnyStatue" + std::to_string (statueIndex);

		SceneObject* object = SceneManager::Instance ()->Current ()->GetObject (objectName);

		_statues.push_back (object);
	}

	for (std::size_t statueIndex = 1; statueIndex < 8; statueIndex += 2) {
		_statues [statueIndex]->SetActive (false);

		_statuesState [statueIndex] = false;
	}

	for (std::size_t doorIndex = 1; doorIndex < 7; doorIndex ++) {
		std::string objectName = "Door" + std::to_string (doorIndex);

		SceneObject* object = SceneManager::Instance ()->Current ()->GetObject (objectName);

		_doors.push_back (object);
	}

	_statueDoorLink [0] = 0; _statueDoorLink [1] = 1;
	_statueDoorLink [4] = 2; _statueDoorLink [5] = 3;

	_doorOpenArc = 70;

	for (std::size_t statueIndex = 0; statueIndex < 8; statueIndex ++) {
		int doorIndex = _statueDoorLink [statueIndex];

		if (doorIndex == -1) {
			continue;
		}

		if (_statuesState [statueIndex] == true) {
			glm::quat doorRot = _doors [doorIndex]->GetTransform ()->GetRotation ();

			glm::vec3 openAxis (0);

			if (doorIndex == 0 || doorIndex == 2) {
				openAxis = glm::vec3 (0, -1, 0);
			} else {
				openAxis = glm::vec3 (0, 1, 0);
			}

			doorRot *= glm::angleAxis (glm::radians (_doorOpenArc), openAxis);

			_doors [doorIndex]->GetTransform ()->SetRotation (doorRot);
		}
	}

	_player = SceneManager::Instance ()->Current ()->GetObject ("Eve");

	_holdStatueIndex = -1;

	_doorIndex = -1;
	_doorOpenSpeed = 90;
	_doorRotation = 0;

	Font* font = Resources::LoadBitmapFont ("Assets/Fonts/Fonts/arial.fnt");

	_textGUI = new TextGUI ("", font, glm::vec2 (0.5f, 0.9f));
	_textGUI->GetTransform ()->SetScale (glm::vec3 (0.7f, 0.7f, 0.0f));
	SceneManager::Instance ()->Current ()->AttachObject (_textGUI);
}

void PuzzleController::Update ()
{
	CheckStatueState ();

	UpdateDoor ();

	if (_textMessageTimer > 0) {
		_textMessageTimer -= Time::GetDeltaTime ();
	}

	if (_textMessageTimer < 0) {

		_textGUI->SetText ("");

		_textMessageTimer = 0;
	}
}

void PuzzleController::CheckStatueState ()
{
	bool isActivation = false;

	if (Input::GetKeyDown (InputKey::E) || Input::GetJoystickButtonDown (2)) {
		isActivation = true;
	}

	if (!isActivation) {
		return;
	}

	const float triggerDistance = 2.5f;

	glm::vec3 playerPos = _player->GetTransform ()->GetPosition ();

	for (std::size_t statueIndex = 0; statueIndex < _statues.size (); statueIndex ++) {

		SceneObject* statue = _statues [statueIndex];

		glm::vec3 objectPos = statue->GetTransform ()->GetPosition ();

		float distance = glm::distance (objectPos, playerPos);

		if (distance < triggerDistance) {

			if (_holdStatueIndex == -1) {

				if (_statuesState [statueIndex] == false) {
					Message ("Nothing to pick");
				} else {
					_holdStatueIndex = statueIndex;

					_statuesState [statueIndex] = false;

					statue->SetActive (false);

					Message ("You took the statue");

					int doorIndex = _statueDoorLink [statueIndex];
					if (doorIndex != -1) {
						CloseDoor (doorIndex);
					}

					if (doorIndex == -1) {
						if (statueIndex == 2 && _statuesState [3] == true) {
							CloseDoor (4);
						}
						if (statueIndex == 3 && _statuesState [2] == true) {
							CloseDoor (4);
						}
						if (statueIndex == 6 && _statuesState [7] == true) {
							CloseDoor (5);
						}
						if (statueIndex == 7 && _statuesState [6] == true) {
							CloseDoor (5);
						}
					}
				}
			}
			else {
				if (_statuesState [statueIndex] == true) {
					Message ("There's already a statue");
				} else {
					_holdStatueIndex = -1;

					_statuesState [statueIndex] = true;

					statue->SetActive (true);

					Message ("You placed the statue");

					int doorIndex = _statueDoorLink [statueIndex];
					if (doorIndex != -1) {
						OpenDoor (doorIndex);
					}

					if (doorIndex == -1) {
						if (statueIndex == 2 && _statuesState [3] == true) {
							OpenDoor (4);
						}
						if (statueIndex == 3 && _statuesState [2] == true) {
							OpenDoor (4);
						}
						if (statueIndex == 6 && _statuesState [7] == true) {
							OpenDoor (5);
						}
						if (statueIndex == 7 && _statuesState [6] == true) {
							OpenDoor (5);
						}
					}
				}
			}

			break;
		}
	}
}

void PuzzleController::UpdateDoor ()
{
	if (_doorIndex == -1) {
		return;
	}

	if (_doorRotation == 0) {
		return;
	}

	glm::vec3 axis (0);

	if (_doorIndex == 0 || _doorIndex == 2 || _doorIndex == 4) {
		axis = glm::vec3 (0, -1, 0);
	} else {
		axis = glm::vec3 (0, 1, 0);
	}

	if (_doorState == DOOR_STATE::DOOR_CLOSE) {
		axis *= -1;
	}

	SceneObject* door = _doors [_doorIndex];

	Transform* doorTransform = door->GetTransform ();

	glm::quat doorRot = doorTransform->GetRotation ();

	float doorRotation = _doorOpenSpeed * Time::GetDeltaTime ();

	if (doorRotation > _doorRotation) {
		doorRotation = _doorRotation;
	}

	_doorRotation -= doorRotation;

	doorRot *= glm::angleAxis (glm::radians (doorRotation), axis);

	doorTransform->SetRotation (doorRot);
}

void PuzzleController::OpenDoor (std::size_t doorIndex)
{
	_doorIndex = doorIndex;

	_doorRotation = _doorOpenArc - _doorRotation;

	_doorState = DOOR_STATE::DOOR_OPEN;
}

void PuzzleController::CloseDoor (std::size_t doorIndex)
{
	_doorIndex = doorIndex;

	_doorRotation = _doorOpenArc - _doorRotation;

	_doorState = DOOR_STATE::DOOR_CLOSE;
}

void PuzzleController::Message (const std::string& text)
{
	_textGUI->SetText (text);

	const float textMessageTime = 5.0f;

	_textMessageTimer = textMessageTime;
}
