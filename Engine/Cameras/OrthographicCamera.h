#ifndef ORTHOGRAPHICCAMERA_H
#define ORTHOGRAPHICCAMERA_H

#include "Systems/Camera/Camera.h"

class OrthographicCamera : public Camera
{
protected:
	float _orthographicSize;

public:
	OrthographicCamera ();

	virtual float GetOrthographicSize () const;

	virtual void SetOrthographicSize (float);

	virtual FrustumVolume* GetFrustumVolume () const;

	virtual glm::mat4 GetProjectionMatrix () const;
};

#endif