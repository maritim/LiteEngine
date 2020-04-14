#ifndef RIGIDBODYCOMPONENT_H
#define RIGIDBODYCOMPONENT_H

#include "Systems/Components/Component.h"

#include <bullet/BulletDynamics/Dynamics/btRigidBody.h>

#include "Systems/Collision/BulletCollider.h"

class ENGINE_API RigidBodyComponent : public Component
{
	DECLARE_COMPONENT(RigidBodyComponent)

protected:
	ATTRIBUTE(EditAnywhere, Meta)
	float _mass;
	ATTRIBUTE(EditAnywhere, Meta=(Collider))
	BulletCollider* _collider;

	btMotionState* _motionState;
	btRigidBody* _rigidBody;

	bool _isSelected;
	bool _isSelectedLastFrame;

public:
	RigidBodyComponent ();
	~RigidBodyComponent ();

	void Awake ();

	void Update ();

	void SetActive (bool isActive);

	void OnAttachedToScene ();
	void OnDetachedFromScene ();

	void OnGizmo ();

	float GetMass () const;
	BulletCollider* GetCollider () const;
	glm::vec3 GetVelocity () const;
	glm::vec3 GetAngularVelocity () const;

	void SetMass (float mass);
	void SetCollider (BulletCollider* collider);
	void SetVelocity (const glm::vec3& velocity);
	void SetAngularVelocity (const glm::vec3& velocity);
protected:
	void UpdateCollider ();
};

#endif