#ifndef PERSPECTIVECAMERA_H
#define PERSPECTIVECAMERA_H

#include "Systems/Camera/Camera.h"

class ENGINE_API PerspectiveCamera : public Camera
{
protected:
	float _fieldOfViewAngle;

public:
	PerspectiveCamera ();

	float GetFieldOfViewAngle () const;

	void SetFieldOfViewAngle (float FOV);

	FrustumVolume GetFrustumVolume () const;

	glm::mat4 GetProjectionMatrix () const;
};

#endif