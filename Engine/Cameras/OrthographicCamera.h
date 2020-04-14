#ifndef ORTHOGRAPHICCAMERA_H
#define ORTHOGRAPHICCAMERA_H

#include "Systems/Camera/Camera.h"

class OrthographicCamera : public Camera
{
protected:
	float _orthographicSize;
	float _leftLimit, _bottomLimit;

public:
	OrthographicCamera ();

	virtual float GetOrthographicSize () const;

	virtual void SetOrthographicSize (float);
	virtual void SetOrthographicInfo (float left, float right, float bottom, float top, float zNear, float zFar);

	virtual FrustumVolume GetFrustumVolume () const;

	virtual glm::mat4 GetProjectionMatrix () const;
};

#endif