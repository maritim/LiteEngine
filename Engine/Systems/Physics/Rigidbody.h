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

	float _mass;
	BulletCollider* _collider;

	bool _isActive;

public:
	Rigidbody ();
	~Rigidbody ();

	void Init ();

	void SetTransform (Transform* transform);
	void SetMass (float mass);
	void SetCollider (BulletCollider* collider);
	void SetVelocity (const glm::vec3& velocity);
	void SetAngularVelocity (const glm::vec3& velocity);
	void SetActive (bool isEnabled);

	float GetMass () const;
	BulletCollider* GetCollider () const;
	glm::vec3 GetVelocity () const;
	glm::vec3 GetAngularVelocity () const;
	bool IsActive () const;

	void Update ();

	void OnAttachedToScene ();
	void OnDetachedFromScene ();

	void Debug (bool isEnabled);
protected:
	void UpdateCollider ();

	void DestroyRigidbody ();
};

#endif