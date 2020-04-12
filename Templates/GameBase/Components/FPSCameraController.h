#ifndef FPSCAMERACONTROLLER_H
#define FPSCAMERACONTROLLER_H

#include "Systems/Components/Component.h"

#include <glm/vec3.hpp>

class FPSCameraController : public Component
{
	DECLARE_COMPONENT(FPSCameraController)

private:
	float _pitch;
	float _yaw;

public:
	void Start ();
	
	void Update ();
};

#endif