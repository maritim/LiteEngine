#ifndef RIGIDBODYCOMPONENT_H
#define RIGIDBODYCOMPONENT_H

#include "Systems/Components/Component.h"

#include <bullet/BulletDynamics/Dynamics/btRigidBody.h>

#include "Components/Collision/ColliderComponent.h"

class ENGINE_API RigidBodyComponent : public Component
{
	DECLARE_COMPONENT(RigidBodyComponent)

protected:
	ATTRIBUTE(EditAnywhere, Meta)
	float _mass;

	ColliderComponent* _colliderComponent;

	btMotionState* _motionState;
	btRigidBody* _rigidBody;

public:
	RigidBodyComponent ();
	~RigidBodyComponent ();

	void Awake ();

	void Update ();

	void SetActive (bool isActive);

	void OnAttachedToScene ();
	void OnDetachedFromScene ();

	float GetMass () const;
	glm::vec3 GetVelocity () const;
	glm::vec3 GetAngularVelocity () const;

	void SetMass (float mass);
	void SetVelocity (const glm::vec3& velocity);
	void SetAngularVelocity (const glm::vec3& velocity);
protected:
	void UpdateCollider ();
};

#endif