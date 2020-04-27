#ifndef CAMERACOMPONENT_H
#define CAMERACOMPONENT_H

#include "Systems/Components/Component.h"

#include "Systems/Camera/Camera.h"

class ENGINE_API CameraComponent : public Component
{
	DECLARE_COMPONENT(CameraComponent)

protected:
	ATTRIBUTE(EditAnywhere, Meta)
	float _aspect;
	ATTRIBUTE(EditAnywhere, Meta)
	float _zNear;
	ATTRIBUTE(EditAnywhere, Meta)
	float _zFar;
	ATTRIBUTE(EditAnywhere, Meta)
	bool _constrainAspect;

	Camera* _camera;

public:
	CameraComponent ();
	~CameraComponent ();

	void Update ();

	void SetActive (bool isActive);

	void OnAttachedToScene ();
	void OnDetachedFromScene ();

	float GetAspect () const;
	float GetZNear () const;
	float GetZFar () const;
	bool GetConstrainAspect () const;

	void SetZNear (float zNear);
	void SetZFar (float zFar);
	void SetAspect (float aspect);
	void SetConstrainAspect (bool constraintAspect);
};

#endif