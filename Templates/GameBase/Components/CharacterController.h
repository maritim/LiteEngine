#ifndef CHARACTERCONTROLLER_H
#define CHARACTERCONTROLLER_H

#include "Systems/Components/Component.h"

#include <glm/vec3.hpp>

// #include "SceneNodes/AnimationGameObject.h"

class ENGINE_API CharacterController : public Component
{
	DECLARE_COMPONENT(CharacterController)

protected:
	// AnimationGameObject* _object;

	glm::vec3 _velocity;
	float _currentSpeed;

	float _walkSpeed;
	float _walkBackSpeed;
	float _runSpeed;

	float _rotationSpeed;

	float _accelerationSpeed;

	bool _needJump;
	float _jumpElapsedTime;

	bool _needTurnLeft, _needTurnRight;

	bool _inJump;

	bool _isIdle, _isWalking;
	bool _isWalkingBack, _isRunning;
	float _speed;

public:
	void Start ();
	
	void Update ();
protected:
	void MovePlayer ();
	void AnimatePlayer ();
};

#endif