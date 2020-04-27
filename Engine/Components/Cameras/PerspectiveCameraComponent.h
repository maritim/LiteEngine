#ifndef PERSPECTIVECAMERACOMPONENT_H
#define PERSPECTIVECAMERACOMPONENT_H

#include "CameraComponent.h"

class ENGINE_API PerspectiveCameraComponent : public CameraComponent
{
	DECLARE_COMPONENT(PerspectiveCameraComponent)

protected:
	ATTRIBUTE(EditAnywhere, Meta)
	float _fieldOfViewAngle;

public:
	PerspectiveCameraComponent ();

	void Awake ();

	float GetFieldOfViewAngle () const;

	void SetFieldOfViewAngle (float FOV);
};

#endif