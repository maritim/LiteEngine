#ifndef PHYSICSMANAGER_H
#define PHYSICSMANAGER_H

#include "Core/Singleton/Singleton.h"

#include "bullet/btBulletDynamicsCommon.h"

class PhysicsManager : public Singleton<PhysicsManager>
{
	friend Singleton<PhysicsManager>;

private:
	btDiscreteDynamicsWorld* _dynamicsWorld;

public:
	void Init ();

	void AttachRigidbody (btRigidBody* rigidbody);
	void DetachRigidbody (btRigidBody* rigidbody);

	void Update ();
private:
	PhysicsManager ();
	~PhysicsManager ();
	PhysicsManager (const PhysicsManager&);
	PhysicsManager& operator=(const PhysicsManager&);
};

#endif