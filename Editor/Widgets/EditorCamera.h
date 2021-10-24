#ifndef EDITORCAMERA_H
#define EDITORCAMERA_H

#include "EditorWidget.h"
#include "EditorManager.h"

#include <glm/vec2.hpp>

class EditorCamera : public EditorWidget
{
protected:
	float _pitch;
	float _yaw;

	glm::ivec2 _startMousePosition;

public:
	EditorCamera ();

	void Show ();
};

REGISTER_EDITOR_WIDGET(EditorCamera)

#endif