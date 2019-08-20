#ifndef RIGIDBODY_H
#define RIGIDBODY_H

#include "Core/Interfaces/Object.h"

#include <bullet/BulletDynamics/Dynamics/btRigidBody.h>

#include "SceneGraph/Transform.h"

#include "Systems/Collision/BulletCollider.h"

class ENGINE_API Rigidbody : public Object
{
protected:
	btMotionState* _motionState;
	btRigidBody* _rigidBody;
	Transform* _transform;
	bool _isAttached;

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

	void OnAttachedToScene ();
	void OnDetachedFromScene ();

	void Debug (bool isEnabled);
protected:
	void UpdateCollider ();

	void DestroyRigidbody ();
};

#endif