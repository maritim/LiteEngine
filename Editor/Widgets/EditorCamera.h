#ifndef EDITORCAMERA_H
#define EDITORCAMERA_H

#include "EditorWidget.h"
#include "EditorManager.h"

#include <glm/vec2.hpp>

#include "Systems/Camera/Camera.h"

class EditorCamera : public EditorWidget
{
protected:
	float _pitch;
	float _yaw;

	glm::ivec2 _startMousePosition;

	bool _firstTime;

public:
	EditorCamera ();

	void Show ();
protected:
	void InitCameraState (Camera* camera);

	void SaveCameraState (const Camera* camera);
};

REGISTER_EDITOR_WIDGET(EditorCamera)

#endif