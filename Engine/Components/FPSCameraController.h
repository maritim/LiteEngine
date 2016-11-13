#ifndef FPSCAMERACONTROLLER_H
#define FPSCAMERACONTROLLER_H

#include "Systems/Components/Component.h"
#include "Systems/Components/ComponentsFactory.h"

#include "Core/Math/glm/vec3.hpp"

class FPSCameraController : public Component
{
private:
	float _pitch;
	float _yaw;

public:
	void Start ();
	
	void Update ();
};

REGISTER_COMPONENT(FPSCameraController)

#endif