#ifndef ANIMATIONDEBUGVIEWCONTROLLER_H
#define ANIMATIONDEBUGVIEWCONTROLLER_H

#include "Systems/Components/Component.h"
#include "Systems/Components/ComponentsFactory.h"

#include <vector>

class AnimationDebugViewController : public Component
{
protected:
	int _currentAnimIndex;
	bool _isBlending;
	float _blendDuration;

public:
	void Start ();

	void Update ();
protected:
	void ShowAnimationsWindow ();
};

REGISTER_COMPONENT(AnimationDebugViewController)

#endif