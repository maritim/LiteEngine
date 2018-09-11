#ifndef RIGIDBODY_H
#define RIGIDBODY_H

#include "Core/Interfaces/Object.h"

#include <bullet/BulletDynamics/Dynamics/btRigidBody.h>

#include "SceneGraph/Transform.h"

#include "Systems/Collision/BulletCollider.h"

class Rigidbody : public Object
{
protected:
	btRigidBody* _rigidBody;
	Transform* _transform;

	float _mass;
	BulletCollider* _collider;

public:
	Rigidbody (Transform* _transform);
	~Rigidbody ();

	void SetMass (float mass);
	void SetCollider (BulletCollider* collider);
	void SetVelocity (const glm::vec3& velocity);
	void SetAngularVelocity (const glm::vec3& velocity);

	float GetMass () const;
	BulletCollider* GetCollider () const;
	glm::vec3 GetVelocity () const;
	glm::vec3 GetAngularVelocity () const;

	void Rebuild ();
protected:
	void DestroyRigidbody ();
};

#endif