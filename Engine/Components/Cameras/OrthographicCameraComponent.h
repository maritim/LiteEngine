#ifndef ORTHOGRAPHICCAMERACOMPONENT_H
#define ORTHOGRAPHICCAMERACOMPONENT_H

#include "CameraComponent.h"

class ENGINE_API OrthographicCameraComponent : public CameraComponent
{
	DECLARE_COMPONENT(OrthographicCameraComponent)

protected:
	ATTRIBUTE(EditAnywhere, Meta)
	float _width;

public:
	OrthographicCameraComponent ();

	void Awake ();

	float GetWidth () const;

	void SetWidth (float);
};

#endif