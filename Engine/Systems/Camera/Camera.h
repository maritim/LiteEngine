#ifndef CAMERA_H
#define CAMERA_H

#include "Core/Interfaces/Object.h"

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/quaternion.hpp>

#include "Core/Intersections/FrustumVolume.h"

class ENGINE_API Camera : public Object
{
protected:
	glm::vec3 _position;
	glm::quat _rotation;

	float _aspect;
	float _zNear;
	float _zFar;

	bool _constraintAspect;

public:
	Camera(void);
	virtual ~Camera ();

	virtual glm::vec3 GetPosition () const;
	virtual glm::quat GetRotation () const;

	virtual float GetAspect () const;
	virtual float GetZNear () const;
	virtual float GetZFar () const;
	virtual bool GetConstraintAspect () const;

	virtual void SetPosition (const glm::vec3& position);
	virtual void SetRotation (const glm::quat& rotation);
	virtual void SetRotation (const glm::vec3& eulerAngles);

	virtual void SetZNear (float zNear);
	virtual void SetZFar (float zFar);
	virtual void SetAspect (float aspect);
	virtual void SetConstraintAspect (bool constraintAspect);

	virtual glm::vec3 GetForward () const;
	virtual glm::vec3 GetUp () const;
	virtual glm::vec3 GetLeft () const;

	void Rotate (const glm::vec3&);
	void Rotate (float, const glm::vec3&);
	void Rotate (const glm::quat& rotation);

	virtual void Update ();

	virtual FrustumVolume GetFrustumVolume () const = 0;

	virtual glm::mat4 GetProjectionMatrix () const = 0;
};

#endif