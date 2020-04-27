#include "PerspectiveCameraComponent.h"

#include "SceneGraph/SceneObject.h"

#include "Cameras/PerspectiveCamera.h"

PerspectiveCameraComponent::PerspectiveCameraComponent () :
	_fieldOfViewAngle (45.0f)
{
	_camera = new PerspectiveCamera ();

	_zNear = 0.3f;
	_zFar = 100.0f;
	_constrainAspect = true;
}

void PerspectiveCameraComponent::Awake ()
{
	_camera->SetPosition (_parent->GetTransform ()->GetPosition ());
	_camera->SetRotation (_parent->GetTransform ()->GetRotation ());

	_camera->SetAspect (_aspect);
	_camera->SetZNear (_zNear);
	_camera->SetZFar (_zFar);

	auto perspectiveCamera = (PerspectiveCamera*) _camera;

	perspectiveCamera->SetFieldOfViewAngle (_fieldOfViewAngle);

	OnAttachedToScene ();
}

float PerspectiveCameraComponent::GetFieldOfViewAngle () const
{
	return _fieldOfViewAngle;
}

void PerspectiveCameraComponent::SetFieldOfViewAngle (float FOV)
{
	_fieldOfViewAngle = FOV;

	auto perspectiveCamera = (PerspectiveCamera*) _camera;

	perspectiveCamera->SetFieldOfViewAngle (_fieldOfViewAngle);
}
