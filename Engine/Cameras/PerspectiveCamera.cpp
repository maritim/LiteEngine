#include "PerspectiveCamera.h"

#include <glm/gtc/matrix_transform.hpp>

PerspectiveCamera::PerspectiveCamera () :
	Camera (),
	_fieldOfViewAngle (0.0f)
{

}

float PerspectiveCamera::GetFieldOfViewAngle () const
{
	return _fieldOfViewAngle;
}

void PerspectiveCamera::SetFieldOfViewAngle (float FOV)
{
	_fieldOfViewAngle = FOV * 3.14 / 180;
}

FrustumVolume PerspectiveCamera::GetFrustumVolume () const
{
	glm::mat4 projection = glm::perspective (_fieldOfViewAngle, _aspect, _zNear, _zFar);

	glm::mat4 view = glm::mat4_cast (GetRotation ());
	view = glm::translate (view, GetPosition () * -1.0f);

	glm::mat4 mvp = projection * view;

	return FrustumVolume (mvp);
}

glm::mat4 PerspectiveCamera::GetProjectionMatrix () const
{
	glm::mat4 projection = glm::perspective (_fieldOfViewAngle, _aspect, _zNear, _zFar);

	return projection;
}