#ifndef PUZZLECONTROLLER_H
#define PUZZLECONTROLLER_H

#include "Systems/Components/Component.h"
#include "Systems/Components/ComponentsFactory.h"

#include "SceneGraph/SceneObject.h"

#include "SceneNodes/TextGUI.h"

class PuzzleController : public Component
{
protected:
	std::vector<SceneObject*> _statues;
	std::vector<SceneObject*> _doors;

	std::vector<bool> _statuesState;

	SceneObject* _player;

	std::vector<int> _statueDoorLink;

	int _holdStatueIndex;

	TextGUI* _textGUI;
	float _textMessageTimer;

	int _doorIndex;
	float _doorOpenSpeed;
	float _doorRotation;
	float _doorOpenArc;

	enum DOOR_STATE { DOOR_CLOSE = 1, DOOR_OPEN };

	DOOR_STATE _doorState;

public:
	void Start ();
	
	void Update ();
protected:
	void CheckStatueState ();
	void UpdateDoor ();

	void OpenDoor (std::size_t doorIndex);
	void CloseDoor (std::size_t doorIndex);

	void Message (const std::string& text);
};

REGISTER_COMPONENT(PuzzleController)

#endif