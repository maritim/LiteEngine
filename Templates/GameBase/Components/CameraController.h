#ifndef CAMERACONTROLLER_H
#define CAMERACONTROLLER_H

#include "Systems/Components/Component.h"

#include "SceneGraph/SceneObject.h"

class GAME_API CameraController : public Component
{
	DECLARE_COMPONENT(CameraController)

protected:
	SceneObject* _object;

	glm::vec3 _cameraOffset;

	float _cameraMoveSpeed;
	float _cameraRotSpeed;

	float _recalibrationTime;

	float _playerRadius;

public:
	void Start ();
	
	void Update ();
protected:
	void MoveCamera ();
	void RotateCamera ();

	glm::vec3 GetPlayerPosition ();
};

#endif