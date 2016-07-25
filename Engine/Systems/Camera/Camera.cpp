#include "Camera.h"

#include <cmath>

Camera::Camera(void) :
	_type (Type::PERSPECTIVE),
	_position (),
	_pitch(0),
	_yaw(0),
	_roll(0),
	_fieldOfViewAngle (45),
	_aspect (0.75),
	_zNear (0.3),
	_zFar (100.0)
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

float Camera::GetPitch () const
{
	return _pitch;
}

float Camera::GetYaw () const
{
	return _yaw;
} 

float Camera::GetRoll () const
{
	return _roll;
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

void Camera::SetPosition (glm::vec3 pos)
{
	_position = pos;
}

void Camera::SetPitch (float pitch)
{
	_pitch = pitch;
}

void Camera::SetYaw (float yaw)
{
	_yaw = yaw;
}

void Camera::SetRoll (float roll)
{
	_roll = roll;
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

glm::vec3 Camera::ToVector3(void)
{
	glm::vec3 result;

	result.x = cos (_yaw) * cos (_pitch);
	result.y = sin (_pitch);
	result.z = sin (_yaw) * cos (_pitch);

	return result;
}
