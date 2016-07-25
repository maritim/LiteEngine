#ifndef CAMERA_H
#define CAMERA_H

#include "Core/Interfaces/Object.h"

#include "Core/Math/glm/vec3.hpp"

class Camera : public Object
{
public:
	enum Type {PERSPECTIVE, ORTOGRAPHIC};

private:
	static Camera* _main;
	static Camera* _active;

protected:
	Type _type;

	glm::vec3 _position;
	float _pitch;
	float _yaw;
	float _roll;

	float _fieldOfViewAngle;
	float _aspect;
	float _zNear;
	float _zFar;

public:
	Camera(void);
	~Camera ();

	static Camera* Main ();
	// static Camera* Active ();

	// void Activate ();
	// void SetMain ();

	Type GetType () const;

	glm::vec3 GetPosition () const;
	float GetPitch () const;
	float GetYaw () const;
	float GetRoll () const;
	float GetFieldOfView () const;
	float GetAspect () const;
	float GetZNear () const;
	float GetZFar () const;

	void SetType (Type type);

	void SetPosition (glm::vec3 position);
	void SetPitch (float pitch);
	void SetYaw (float yaw);
	void SetRoll (float roll);
	void SetFieldOfView (float FOV);
	void SetAspect (float aspect);
	void SetZNear (float zNear);
	void SetZFar (float zFar);

	glm::vec3 ToVector3(void);
};

#endif