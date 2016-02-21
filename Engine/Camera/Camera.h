#ifndef CAMERA_H
#define CAMERA_H

#include "Core/Math/Vector3.h"

class Camera
{
private:
	Vector3 _position;
	float _pitch;
	float _yaw;
	float _roll;

public:
	Camera(void);
	~Camera ();

	static Camera* Main ();

	Vector3 GetPosition () const;
	float GetPitch () const;
	float GetYaw () const;
	float GetRoll () const;

	void SetPosition (Vector3 position);
	void SetPitch (float pitch);
	void SetYaw (float yaw);
	void SetRoll (float roll);

	Vector3 ToVector3(void);
};

#endif