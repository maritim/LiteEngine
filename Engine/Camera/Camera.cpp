#include "Camera.h"

#include <cmath>

#ifndef PI
#define PI 3.14159265
#endif

Camera::Camera(void) :
	_position (),
	_pitch(0),
	_yaw(0),
	_roll(0)
{
}

Camera::~Camera ()
{

}

Camera* Camera::Main ()
{
	static Camera* main = new Camera ();

	return main;
}

Vector3 Camera::GetPosition () const
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

void Camera::SetPosition (Vector3 pos)
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

Vector3 Camera::ToVector3(void)
{
	Vector3 result;

	result.x = cos (_yaw) * cos (_pitch);
	result.y = sin (_pitch);
	result.z = sin (_yaw) * cos (_pitch);

	return result;
}