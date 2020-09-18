#ifndef FLYCAMERACONTROLLER_H
#define FLYCAMERACONTROLLER_H

#include "Systems/Components/Component.h"

class GAME_API FlyCameraController : public Component
{
	DECLARE_COMPONENT(FlyCameraController)

private:
	float _pitch;
	float _yaw;

public:
	void Start ();
	
	void Update ();
};

#endif