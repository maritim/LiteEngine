#ifndef RIGIDBODYCOMPONENT_H
#define RIGIDBODYCOMPONENT_H

#include "Systems/Components/Component.h"

#include "Systems/Physics/Rigidbody.h"

class ENGINE_API RigidBodyComponent : public Component
{
	DECLARE_COMPONENT(RigidBodyComponent)

protected:
	ATTRIBUTE(EditAnywhere, Meta)
	float _mass;
	ATTRIBUTE(EditAnywhere, Meta=(Collider))
	BulletCollider* _collider;

	Rigidbody* _rigidbody;

public:
	RigidBodyComponent ();
	~RigidBodyComponent ();

	void Awake ();

	void Update ();

	void SetActive (bool isActive);

	void OnAttachedToScene ();
	void OnDetachedFromScene ();
};

#endif