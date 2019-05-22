#include "Rigidbody.h"

#include "Systems/Physics/PhysicsManager.h"
#include "Systems/Physics/MotionState.h"

Rigidbody::Rigidbody (Transform* transform) :
	_motionState (nullptr),
	_rigidBody (nullptr),
	_transform (transform),
	_mass (0.0f),
	_collider (nullptr),
	_isEnabled (true)
{
	Build ();
}

Rigidbody::~Rigidbody ()
{
	DestroyRigidbody ();

	if (_collider != nullptr) {
		delete _collider;
	}
}

void Rigidbody::SetMass (float mass)
{
	_mass = mass;

	btVector3 localInertia = btVector3 (0.0f, 0.0f, 0.0f);

	if (_collider != nullptr) {
		_collider->GetCollisionShape ()->calculateLocalInertia (_mass, localInertia);
	}

	_rigidBody->setMassProps (mass, localInertia);
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
}

void Rigidbody::SetVelocity (const glm::vec3& velocity)
{
	_rigidBody->setLinearVelocity (btVector3 (velocity.x, velocity.y, velocity.z));
}

void Rigidbody::SetAngularVelocity (const glm::vec3& velocity)
{
	_rigidBody->setAngularVelocity (btVector3 (velocity.x, velocity.y, velocity.z));
}

void Rigidbody::Enable (bool isEnabled)
{
	_isEnabled = isEnabled;

	int flags = _rigidBody->getFlags ();
	flags = _isEnabled ? flags &~ ISLAND_SLEEPING : flags | ISLAND_SLEEPING;

	_rigidBody->setFlags (flags);
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
	btVector3 velocity = _rigidBody->getLinearVelocity ();

	return glm::vec3 (velocity.getX (), velocity.getY (), velocity.getZ ());
}

glm::vec3 Rigidbody::GetAngularVelocity () const
{
	btVector3 velocity = _rigidBody->getAngularVelocity ();

	return glm::vec3 (velocity.getX (), velocity.getY (), velocity.getZ ());
}

bool Rigidbody::IsEnabled () const
{
	return _isEnabled;
}

void Rigidbody::Update ()
{
	btTransform transform;

	glm::vec3 offset = _collider == nullptr ? glm::vec3 (0.0f) : _transform->GetRotation () * _collider->GetOffset ();

	glm::vec3 position = _transform->GetPosition () + offset;
	glm::quat rotation = _transform->GetRotation ();

	transform.setOrigin (btVector3 (position.x, position.y, position.z));
	transform.setRotation (btQuaternion (rotation.x, rotation.y, rotation.z, rotation.w));

	_rigidBody->setWorldTransform (transform);

	/*
	 * Set collision shape scaling
	*/

	if (_collider != nullptr) {
		glm::vec3 scale = _transform->GetScale ();

		_collider->GetCollisionShape ()->setLocalScaling (btVector3 (scale.x, scale.y, scale.z));		
	}

	/*
	 * Update collider
	*/

	if (_collider != nullptr && _collider->IsDirty ()) {
		UpdateCollider ();
	}
}

void Rigidbody::OnAttachedToScene ()
{
	UpdateCollider ();

	/*
	 * Attach rigid body to physics system
	*/

	PhysicsManager::Instance ()->AttachRigidbody (_rigidBody);
}

void Rigidbody::OnDetachedFromScene ()
{
	/*
	 * Detach rigid body from the physics system
	*/

	PhysicsManager::Instance ()->DetachRigidbody (_rigidBody);
}

void Rigidbody::Debug (bool isEnabled)
{
	int collisionFlags = _rigidBody->getCollisionFlags ();

	int newCollisionFlags = isEnabled ?
		collisionFlags &~ btCollisionObject::CF_DISABLE_VISUALIZE_OBJECT :
		collisionFlags | btCollisionObject::CF_DISABLE_VISUALIZE_OBJECT;

	_rigidBody->setCollisionFlags (newCollisionFlags);
}

void Rigidbody::Build ()
{
	_motionState = new MotionState (_transform, glm::vec3 (0.0f)); //_collider->GetOffset ());

	/*
	 * Initialize rigidbody
	*/

	btVector3 localInertia = btVector3 (0.0f, 0.0f, 0.0f);

	btRigidBody::btRigidBodyConstructionInfo rigidBodyCI (
		_mass,
		_motionState,
		nullptr,
		localInertia
	);

	_rigidBody = new btRigidBody (rigidBodyCI);

	/*
	 * Disable debug data
	*/

	_rigidBody->setCollisionFlags (_rigidBody->getCollisionFlags ()
		| btCollisionObject::CF_DISABLE_VISUALIZE_OBJECT);
}

void Rigidbody::UpdateCollider ()
{
	if (_collider == nullptr) {
		return;
	}

	/*
	 * Set collision shape scaling
	*/

	glm::vec3 scale = _transform->GetScale ();

	_collider->GetCollisionShape ()->setLocalScaling (btVector3 (scale.x, scale.y, scale.z));

	/*
	 * Compute local inertia based on collision shape
	 *
	 * Default local inertia is vec3 (0, 0, 0),
	 * which is fixed in respect to any axis rotation
	*/

	btVector3 localInertia = btVector3 (0.0f, 0.0f, 0.0f);

	_collider->GetCollisionShape ()->calculateLocalInertia (_mass, localInertia);

	/*
	 * Update rigidbody local inertia
	*/

	_rigidBody->setMassProps (_mass, localInertia);

	/*
	 * Update rigidbody collision shape
	*/

	_rigidBody->setCollisionShape (_collider->GetCollisionShape ());
}

void Rigidbody::DestroyRigidbody ()
{
	if (_rigidBody != nullptr) {
		delete _rigidBody;
	}

	if (_motionState != nullptr) {
		delete _motionState;
	}
}