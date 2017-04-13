#include "Camera.h"

#include "Utils/Extensions/MathExtend.h"

#include <cmath>

/*
 * TODO: Change this somewhere else
*/
 
#include "Cameras/PerspectiveCamera.h"
#include "Cameras/OrthographicCamera.h"

Camera::Camera(void) :
	_position (),
	_rotation (),
	_aspect (0.0f),
	_zNear (0.0f),
	_zFar (1.0f)
{

}

Camera::~Camera ()
{

}

Camera* Camera::_main (nullptr);

Camera* Camera::Main ()
{
	/*
	 * TODO: Change this somewhere else
	*/
 
	if (_main == nullptr) {
		_main = new PerspectiveCamera ();
	}

	return _main;
}

// Camera* Camera::Active ()
// {
// 	if (_active == nullptr) {
// 		_active = Camera::Main ();
// 	}

// 	return _active;
// }

// void Camera::Activate ()
// {
	
// }

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