#ifndef FRAMERATE_H
#define FRAMERATE_H

#include "Systems/Components/Component.h"

#include "SceneGraph/Scene.h"

#include "SceneNodes/TextGUI.h"

class FrameRate : public Component
{
	DECLARE_COMPONENT(FrameRate)

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

#endif