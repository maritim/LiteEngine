#ifndef EDITORCAMERA_H
#define EDITORCAMERA_H

#include "EditorWidget.h"
#include "EditorManager.h"

class EditorCamera : public EditorWidget
{
protected:
	float _pitch;
	float _yaw;

public:
	EditorCamera ();

	void Show ();
};

REGISTER_EDITOR_WIDGET(EditorCamera)

#endif