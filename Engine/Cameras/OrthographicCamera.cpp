#include "OrthographicCamera.h"

#include "Core/Math/glm/gtc/matrix_transform.hpp"

OrthographicCamera::OrthographicCamera () :
	Camera (),
	_orthographicSize (20.0f)
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

FrustumVolume* OrthographicCamera::GetFrustumVolume () const
{
	glm::mat4 projection = glm::perspective (90.0f, 1.0f, _zNear, _zFar);

	glm::mat4 view = glm::mat4_cast (GetRotation ());
	view = glm::translate (view, GetPosition () * -1.0f);

	glm::mat4 mvp = projection * view;

	FrustumVolume* frustum = new FrustumVolume (mvp);

	return frustum;
}

glm::mat4 OrthographicCamera::GetProjectionMatrix () const
{
	float horizontalSize = _orthographicSize * _aspect;
	float verticalSize = _orthographicSize;

	glm::mat4 projectionMatrix = glm::ortho (0.0f, horizontalSize, 0.0f, verticalSize, _zNear, _zFar);

	return projectionMatrix;
}
