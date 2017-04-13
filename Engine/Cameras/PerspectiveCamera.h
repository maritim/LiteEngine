#ifndef PERSPECTIVECAMERA_H
#define PERSPECTIVECAMERA_H

#include "Systems/Camera/Camera.h"

class PerspectiveCamera : public Camera
{
protected:
	float _fieldOfViewAngle;

public:
	PerspectiveCamera ();

	float GetFieldOfView () const;

	void SetFieldOfView (float FOV);

	FrustumVolume* GetFrustumVolume () const;

	glm::mat4 GetProjectionMatrix () const;
};

#endif