#include "OrthographicCameraComponent.h"

#include "SceneGraph/SceneObject.h"

#include "Cameras/OrthographicCamera.h"

OrthographicCameraComponent::OrthographicCameraComponent () :
	_width (10.0f)
{
	_camera = new OrthographicCamera ();

	_zNear = 0.3f;
	_zFar = 100.0f;
	_constrainAspect = true;
}

void OrthographicCameraComponent::Awake ()
{
	_camera->SetPosition (_parent->GetTransform ()->GetPosition ());
	_camera->SetRotation (_parent->GetTransform ()->GetRotation ());

	_camera->SetAspect (_aspect);
	_camera->SetZNear (_zNear);
	_camera->SetZFar (_zFar);

	SetWidth (_width);

	OnAttachedToScene ();
}

float OrthographicCameraComponent::GetWidth () const
{
	return _width;
}

void OrthographicCameraComponent::SetWidth (float width)
{
	_width = width;

	auto orthographicCamera = (OrthographicCamera*) _camera;

	orthographicCamera->SetOrthographicSize (_width);
}
