#ifndef FRAMERATE_H
#define FRAMERATE_H

#include "SceneGraph/SceneObject.h"
#include "SceneGraph/Scene.h"

#include "SceneNodes/TextGUI.h"

class FrameRate : public SceneObject
{
protected:
	TextGUI* _textGUI;
public:
	FrameRate (Scene* currentScene);

	void Update();
	void Draw();

	~FrameRate();
};

#endif