#ifndef PHYSICSMANAGER_H
#define PHYSICSMANAGER_H

#include "Core/Singleton/Singleton.h"

#include "bullet/btBulletDynamicsCommon.h"

#include "RaycastProduct.h"

class ENGINE_API PhysicsManager : public Singleton<PhysicsManager>
{
	friend Singleton<PhysicsManager>;

	DECLARE_SINGLETON(PhysicsManager)

private:
	btDefaultCollisionConfiguration* _collisionConfiguration;
	btCollisionDispatcher* _collisionDispatcher;
	btDbvtBroadphase* _broadphaseInterface;
	btSequentialImpulseConstraintSolver* _solver;
	btDiscreteDynamicsWorld* _dynamicsWorld;

public:
	void Init ();

	void AttachRigidbody (btRigidBody* rigidbody);
	void DetachRigidbody (btRigidBody* rigidbody);

	RaycastProduct Raycast (const glm::vec3& origin, const glm::vec3& direction,
		float distance = 2000);

	void Update ();

	void Clear ();
private:
	PhysicsManager ();
	~PhysicsManager ();
	PhysicsManager (const PhysicsManager&);
	PhysicsManager& operator=(const PhysicsManager&);
};

#endif