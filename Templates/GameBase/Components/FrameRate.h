#ifndef FRAMERATE_H
#define FRAMERATE_H

#include "Systems/Components/Component.h"
#include "Systems/Components/ComponentsFactory.h"

#include "SceneGraph/Scene.h"

#include "SceneNodes/TextGUI.h"

class FrameRate : public Component
{
private:
	float _timeElapsed;
	float _computeRange;
protected:
	TextGUI* _textGUI;

public:
	FrameRate ();
	~FrameRate();

	void Start ();

	void Update();
};

REGISTER_COMPONENT(FrameRate)

#endif