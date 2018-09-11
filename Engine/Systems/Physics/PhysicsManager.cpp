#include "PhysicsManager.h"

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
	if (_dynamicsWorld == nullptr) {
		return;
	}

	delete _dynamicsWorld;
}

void PhysicsManager::Init ()
{
	/*
	 * Create collision configuration
	*/

	btDefaultCollisionConfiguration* collisionConfiguration = new btDefaultCollisionConfiguration ();

	/*
	 * Create collision dispatcher
	*/

	btCollisionDispatcher* collisionDispatcher = new btCollisionDispatcher (collisionConfiguration);

	/*
	 * Create broadphase
	*/

	btBroadphaseInterface* broadphaseInterface = new btDbvtBroadphase ();

	/*
	 * Create solver
	*/

	btSequentialImpulseConstraintSolver* solver = new btSequentialImpulseConstraintSolver ();

	/*
	 * Initialize dynamics world
	*/

	_dynamicsWorld = new btDiscreteDynamicsWorld (
		collisionDispatcher,
		broadphaseInterface,
		solver,
		collisionConfiguration
	);

	/*
	 * Initialize gravity
	*/

	_dynamicsWorld->setGravity (btVector3 (0.0f, -GRAVITATIONAL_ACCELERATION, 0.0f));
}

void PhysicsManager::AttachRigidbody (btRigidBody* rigidbody)
{
	_dynamicsWorld->addRigidBody (rigidbody);
}

void PhysicsManager::DetachRigidbody (btRigidBody* rigidbody)
{
	_dynamicsWorld->removeRigidBody (rigidbody);
}

void PhysicsManager::Update ()
{
	/*
	 * Update physics system based on time passed since last frame
	*/

	float dt = Time::GetDeltaTime ();

	_dynamicsWorld->stepSimulation (dt);
}
