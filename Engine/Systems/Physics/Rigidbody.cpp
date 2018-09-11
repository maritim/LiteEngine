#include "Rigidbody.h"

#include "Systems/Physics/PhysicsManager.h"
#include "Systems/Physics/MotionState.h"

#include "Systems/Time/Time.h"

Rigidbody::Rigidbody (Transform* transform) :
	_rigidBody (nullptr),
	_transform (transform),
	_mass (0.0f),
	_collider (nullptr)
{

}

Rigidbody::~Rigidbody ()
{
	DestroyRigidbody ();
}

void Rigidbody::SetMass (float mass)
{
	_mass = mass;

	Rebuild ();
}

void Rigidbody::SetCollider (BulletCollider* collider)
{
	/*
	 * Destroy current collider if exists
	*/

	if (_collider != nullptr) {
		delete _collider;
	}

	_collider = collider;

	Rebuild ();
}

void Rigidbody::SetVelocity (const glm::vec3& velocity)
{
	if (_rigidBody == nullptr) {
		return;
	}

	_rigidBody->setLinearVelocity (btVector3 (velocity.x, velocity.y, velocity.z));
}

void Rigidbody::SetAngularVelocity (const glm::vec3& velocity)
{
	if (_rigidBody == nullptr) {
		return;
	}

	_rigidBody->setAngularVelocity (btVector3 (velocity.x, velocity.y, velocity.z));
}

float Rigidbody::GetMass () const
{
	return _mass;
}

BulletCollider* Rigidbody::GetCollider () const
{
	return _collider;
}

glm::vec3 Rigidbody::GetVelocity () const
{
	if (_rigidBody == nullptr) {
		return glm::vec3 (0.0f);
	}

	btVector3 velocity = _rigidBody->getLinearVelocity ();

	return glm::vec3 (velocity.getX (), velocity.getY (), velocity.getZ ());
}

glm::vec3 Rigidbody::GetAngularVelocity () const
{
	if (_rigidBody == nullptr) {
		return glm::vec3 (0.0f);
	}

	btVector3 velocity = _rigidBody->getAngularVelocity ();

	return glm::vec3 (velocity.getX (), velocity.getY (), velocity.getZ ());
}

void Rigidbody::Rebuild ()
{
	if (_collider == nullptr) {
		return;
	}

	/*
	 * Destroy current rigidbody if exists
	*/

	DestroyRigidbody ();

	btMotionState* motionState = new MotionState (_transform, _collider->GetOffset ());

	/*
	 * Compute local inertia based on collision shape
	 *
	 * Default local inertia is vec3 (0, 0, 0),
	 * which is fixed in respect to any axis rotation
	*/

	btVector3 localInertia = btVector3 (0.0f, 0.0f, 0.0f);

	if (_collider->GetCollisionShape () != nullptr) {
		_collider->GetCollisionShape ()->calculateLocalInertia (_mass, localInertia);
	}

	/*
	 * Initialize rigidbody
	*/

	btRigidBody::btRigidBodyConstructionInfo rigidBodyCI (
		_mass,
		motionState,
		_collider->GetCollisionShape (),
		localInertia
	);

	_rigidBody = new btRigidBody (rigidBodyCI);

	/*
	 * Detach rigid body from the physics system
	*/

	PhysicsManager::Instance ()->DetachRigidbody (_rigidBody);

	/*
	 * Attach rigid body to physics system
	*/

	PhysicsManager::Instance ()->AttachRigidbody (_rigidBody);
}

void Rigidbody::DestroyRigidbody ()
{
	if (_rigidBody == nullptr) {
		return;
	}

	delete _rigidBody;
}