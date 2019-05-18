#include "Camera.h"

#include "Utils/Extensions/MathExtend.h"

#include <cmath>

#include "Systems/Window/Window.h"

Camera::Camera(void) :
	_position (),
	_rotation (),
	_aspect (0.0f),
	_zNear (0.0f),
	_zFar (1.0f),
	_constraintAspect (true),
	_frustumVolume (new FrustumVolume (new FrustumVolume::FrustumVolumeInformation ()))
{

}

Camera::~Camera ()
{

}

glm::vec3 Camera::GetPosition () const
{
	return _position;
}

glm::quat Camera::GetRotation () const
{
	return _rotation;
}

float Camera::GetAspect () const
{
	return _aspect;
}

float Camera::GetZNear () const
{
	return _zNear;
}

float Camera::GetZFar () const
{
	return _zFar;
}

bool Camera::GetConstraintAspect () const
{
	return _constraintAspect;
}

void Camera::SetPosition (const glm::vec3& pos)
{
	_position = pos;
}

void Camera::SetAspect (float aspect)
{
	_aspect = aspect;
}

void Camera::SetZNear (float zNear)
{
	_zNear = zNear;
}

void Camera::SetZFar (float zFar)
{
	_zFar = zFar;
}

void Camera::SetConstraintAspect (bool constraintAspect)
{
	_constraintAspect = constraintAspect;
}

void Camera::SetRotation (const glm::quat& rotation)
{
	_rotation = glm::normalize (rotation);
}

void Camera::SetRotation(const glm::vec3& eulerAngles)
{
	_rotation = glm::normalize (glm::quat (eulerAngles * DEG2RAD));
}

glm::vec3 Camera::GetForward () const
{
	return glm::conjugate (_rotation) * glm::vec3 (0.0f, 0.0f, -1.0f);
}

glm::vec3 Camera::GetLeft () const
{
	return glm::conjugate (_rotation) * glm::vec3 (-1.0, 0.0f, 0.0f);
}

glm::vec3 Camera::GetUp () const
{
	return glm::conjugate (_rotation) * glm::vec3 (0.0f, 1.0f, 0.0f);
}

void Camera::Rotate (const glm::vec3& angleRadians)
{
	Rotate (angleRadians.x, glm::vec3 (1.0f, 0.0f, 0.0f));
	Rotate (angleRadians.y, glm::vec3 (0.0f, 1.0f, 0.0f));
	Rotate (angleRadians.z, glm::vec3 (0.0f, 0.0f, 1.0f));
}

void Camera::Rotate (float angleRadians, const glm::vec3& axis) 
{
	glm::quat q = glm::angleAxis(angleRadians, axis);
	Rotate(q);
}

void Camera::Rotate (const glm::quat& rotation) 
{
	_rotation = rotation * _rotation;
	_rotation = glm::normalize (_rotation);
}

void Camera::Update ()
{
	if (_constraintAspect == true) {
		_aspect = ((float) Window::GetWidth ()) / Window::GetHeight ();
	}
}
