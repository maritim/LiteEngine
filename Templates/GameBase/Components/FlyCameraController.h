#ifndef FLYCAMERACONTROLLER_H
#define FLYCAMERACONTROLLER_H

#include "Systems/Components/Component.h"
#include "Systems/Components/ComponentsFactory.h"

#include "SceneNodes/TextGUI.h"

class FlyCameraController : public Component
{
private:
	float _pitch;
	float _yaw;

public:
	void Start ();
	
	void Update ();
};

REGISTER_COMPONENT(FlyCameraController)

#endif