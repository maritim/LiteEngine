#include "PerspectiveCamera.h"

#include <glm/gtc/matrix_transform.hpp>

PerspectiveCamera::PerspectiveCamera () :
	Camera (),
	_fieldOfViewAngle (0.0f)
{
	_aspect = 0.0f;
	_zNear = 0.3f;
	_zFar = 100.0f;
}

float PerspectiveCamera::GetFieldOfView () const
{
	return _fieldOfViewAngle;
}

void PerspectiveCamera::SetFieldOfView (float FOV)
{
	_fieldOfViewAngle = FOV * 3.14 / 180;
}

FrustumVolume* PerspectiveCamera::GetFrustumVolume () const
{
	glm::mat4 projection = glm::perspective (_fieldOfViewAngle, _aspect, _zNear, _zFar);

	glm::mat4 view = glm::mat4_cast (GetRotation ());
	view = glm::translate (view, GetPosition () * -1.0f);

	glm::mat4 mvp = projection * view;

	FrustumVolume* frustum = new FrustumVolume (mvp);

	return frustum;
}

glm::mat4 PerspectiveCamera::GetProjectionMatrix () const
{
	glm::mat4 projection = glm::perspective (_fieldOfViewAngle, _aspect, _zNear, _zFar);

	return projection;
}