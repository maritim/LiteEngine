#ifndef CAMERA_H
#define CAMERA_H

#include "Core/Interfaces/Object.h"

#include "Core/Math/glm/vec3.hpp"
#include "Core/Math/glm/gtc/quaternion.hpp"

#include "Core/Intersections/FrustumVolume.h"

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
	glm::quat _rotation;

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
	glm::quat GetRotation () const;
	float GetFieldOfView () const;
	float GetAspect () const;
	float GetZNear () const;
	float GetZFar () const;

	void SetType (Type type);

	void SetPosition (const glm::vec3& position);
	void SetRotation (const glm::quat& rotation);
	void SetRotation (const glm::vec3& eulerAngles);
	void SetFieldOfView (float FOV);
	void SetAspect (float aspect);
	void SetZNear (float zNear);
	void SetZFar (float zFar);

	glm::vec3 GetForward () const;
	glm::vec3 GetUp () const;
	glm::vec3 GetLeft () const;

	void Rotate (const glm::vec3&);
	void Rotate (float, const glm::vec3&);
	void Rotate (const glm::quat& rotation);

	FrustumVolume* GetFrustumVolume () const;
};

#endif