#include "OrthographicCamera.h"

#include <glm/gtc/matrix_transform.hpp>

OrthographicCamera::OrthographicCamera () :
	Camera (),
	_orthographicSize (100.0f),
	_leftLimit (0.0f),
	_bottomLimit (0.0f)
{
	_aspect = 0.75;
	_zNear = 0.3f;
	_zFar = 10000.0f;
}

float OrthographicCamera::GetOrthographicSize () const
{
	return _orthographicSize;
}

void OrthographicCamera::SetOrthographicSize (float orthographicSize)
{
	_orthographicSize = orthographicSize;
}


void OrthographicCamera::SetOrthographicInfo(float left, float right, float bottom, float top, float zNear, float zFar)
{
	_leftLimit = left;

	_bottomLimit = bottom;

	_zNear = zNear;
	_zFar = zFar;

	_orthographicSize = top - _bottomLimit;
	_aspect = (right - _leftLimit) / _orthographicSize;
}

/*
 * Frustum Volume is calculated using the MVP matrix
 * Thanks to: 
 * 
 * 		http://ruh.li/CameraViewFrustum.html
 *
*/

FrustumVolume* OrthographicCamera::GetFrustumVolume () const
{
	glm::mat4 projection = GetProjectionMatrix ();

	glm::mat4 view = glm::mat4_cast (GetRotation ());
	view = glm::translate (view, GetPosition () * -1.0f);

	glm::mat4 mvp = projection * view;

	FrustumVolume* frustum = new FrustumVolume (mvp);

	return frustum;
}

glm::mat4 OrthographicCamera::GetProjectionMatrix () const
{
	float rightLimit = _leftLimit + _orthographicSize * _aspect;
	float topLimit = _bottomLimit + _orthographicSize;

	glm::mat4 projectionMatrix = glm::ortho (_leftLimit, rightLimit, _bottomLimit, topLimit, _zNear, _zFar);

	return projectionMatrix;
}
