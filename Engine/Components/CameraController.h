#ifndef CAMERACONTROLLER_H
#define CAMERACONTROLLER_H

#include "Systems/Components/Component.h"
#include "Systems/Components/ComponentsFactory.h"

class CameraController : public Component
{
public:
	void Start ();
	
	void Update ();
};

REGISTER_COMPONENT(CameraController)

#endif