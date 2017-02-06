#include "Camera.h"

#include "Utils/Extensions/MathExtend.h"

#include <cmath>

#include "Core/Math/glm/gtc/matrix_transform.hpp"

Camera::Camera(void) :
	_type (Type::PERSPECTIVE),
	_position (),
	_rotation (),
	_fieldOfViewAngle (45),
	_aspect (0.75f),
	_zNear (0.3f),
	_zFar (10000.0f)
{
}

Camera::~Camera ()
{

}

Camera* Camera::_main (nullptr);

Camera* Camera::Main ()
{
	if (_main == nullptr) {
		_main = new Camera ();
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

Camera::Type Camera::GetType () const
{
	return _type;
}

glm::vec3 Camera::GetPosition () const
{
	return _position;
}

glm::quat Camera::GetRotation () const
{
	return _rotation;
}

float Camera::GetFieldOfView () const
{
	return _fieldOfViewAngle;
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

void Camera::SetType (Camera::Type type)
{
	_type = type;
}

void Camera::SetPosition (const glm::vec3& pos)
{
	_position = pos;
}

void Camera::SetRotation (const glm::quat& rotation)
{
	_rotation = glm::normalize (rotation);
}

void Camera::SetRotation(const glm::vec3& eulerAngles)
{
	_rotation = glm::normalize (glm::quat (eulerAngles * DEG2RAD));
}

void Camera::SetFieldOfView (float FOV)
{
	_fieldOfViewAngle = FOV;
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

FrustumVolume* Camera::GetFrustumVolume () const
{
	glm::mat4 projection = glm::perspective (_fieldOfViewAngle, _aspect, _zNear, _zFar);

	glm::mat4 view = glm::mat4_cast (GetRotation ());
	view = glm::translate (view, GetPosition () * -1.0f);

	glm::mat4 mvp = projection * view;

	FrustumVolume* frustum = new FrustumVolume (mvp);

	return frustum;
}
