#include "PhysicsManager.h"

#include <glm/geometric.hpp>

#include "BulletDebugDraw.h"

#include "Systems/Time/Time.h"

/*
 * TODO: Change this to somewhere else
*/

#define GRAVITATIONAL_ACCELERATION 2.0f

PhysicsManager::PhysicsManager () :
	_dynamicsWorld (nullptr)
{

}

PhysicsManager::~PhysicsManager ()
{

}

SPECIALIZE_SINGLETON(PhysicsManager)

void PhysicsManager::Init ()
{
	/*
	 * Create collision configuration
	*/

	_collisionConfiguration = new btDefaultCollisionConfiguration ();

	/*
	 * Create collision dispatcher
	*/

	_collisionDispatcher = new btCollisionDispatcher (_collisionConfiguration);

	/*
	 * Create broadphase
	*/

	_broadphaseInterface = new btDbvtBroadphase ();

	/*
	 * Create solver
	*/

	_solver = new btSequentialImpulseConstraintSolver ();

	/*
	 * Initialize dynamics world
	*/

	_dynamicsWorld = new btDiscreteDynamicsWorld (
		_collisionDispatcher,
		_broadphaseInterface,
		_solver,
		_collisionConfiguration
	);

	/*
	 * Initialize gravity
	*/

	_dynamicsWorld->setGravity (btVector3 (0.0f, -GRAVITATIONAL_ACCELERATION, 0.0f));

	/*
	 * Initialize debug mode
	*/

	_debugDraw = new BulletDebugDraw ();
	_dynamicsWorld->setDebugDrawer (_debugDraw);
}

void PhysicsManager::Clear ()
{
	delete _dynamicsWorld;

	delete _solver;
	delete _broadphaseInterface;
	delete _collisionDispatcher;
	delete _collisionConfiguration;
	delete _debugDraw;
}

void PhysicsManager::AttachRigidbody (btRigidBody* rigidbody)
{
	_dynamicsWorld->addRigidBody (rigidbody);
}

void PhysicsManager::DetachRigidbody (btRigidBody* rigidbody)
{
	_dynamicsWorld->removeRigidBody (rigidbody);
}

RaycastProduct PhysicsManager::Raycast (const glm::vec3& origin, const glm::vec3& direction, float distance)
{
	RaycastProduct product;

	/*
	 * Compute ray cast limit
	*/

	glm::vec3 limit = origin + glm::normalize (direction) * distance;

	btVector3 btOrigin = btVector3 (origin.x, origin.y, origin.z);
	btVector3 btLimit = btVector3 (limit.x, limit.y, limit.z);

	/*
	 * Create ray callback
	*/

	btCollisionWorld::ClosestRayResultCallback rayCallback (btOrigin, btLimit);

	/*
	 * Ray test
	*/

	_dynamicsWorld->rayTest (btOrigin, btLimit, rayCallback);

	/*
	 * Check ray callback
	*/

	if (rayCallback.hasHit()) {

		product.isCollision = true;

		product.hitPosition = glm::vec3 (
			rayCallback.m_hitPointWorld.x (),
			rayCallback.m_hitPointWorld.y (),
			rayCallback.m_hitPointWorld.z ()
		);

		product.hitNormal = glm::vec3 (
			rayCallback.m_hitNormalWorld.x (),
			rayCallback.m_hitNormalWorld.y (),
			rayCallback.m_hitNormalWorld.z ()
		);
	}

	return product;
}

void PhysicsManager::Update ()
{
	/*
	 * Update physics system based on time passed since last frame
	*/

	float dt = Time::GetDeltaTime ();

	_dynamicsWorld->stepSimulation (dt);

	_dynamicsWorld->debugDrawWorld ();
}
