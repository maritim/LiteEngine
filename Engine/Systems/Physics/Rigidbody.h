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

	bool _isEnabled;

public:
	Rigidbody (Transform* _transform);
	~Rigidbody ();

	void SetMass (float mass);
	void SetCollider (BulletCollider* collider);
	void SetVelocity (const glm::vec3& velocity);
	void SetAngularVelocity (const glm::vec3& velocity);
	void Enable (bool isEnabled);

	float GetMass () const;
	BulletCollider* GetCollider () const;
	glm::vec3 GetVelocity () const;
	glm::vec3 GetAngularVelocity () const;
	bool IsEnabled () const;

	void Update ();

	void Rebuild ();
protected:
	void DestroyRigidbody ();
};

#endif