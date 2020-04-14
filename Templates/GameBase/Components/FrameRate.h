#ifndef FRAMERATE_H
#define FRAMERATE_H

#include "Systems/Components/Component.h"

#include "Components/TextGUIComponent.h"

class FrameRate : public Component
{
	DECLARE_COMPONENT(FrameRate)

private:
	float _timeElapsed;
	float _computeRange;
protected:
	TextGUIComponent* _textGUIComponent;

public:
	FrameRate ();
	~FrameRate();

	void Start ();

	void Update();
};

#endif