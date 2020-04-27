#include "ColliderComponent.h"

#include <glm/gtc/quaternion.hpp>

#include "Systems/Physics/PhysicsManager.h"

#include "SceneGraph/SceneObject.h"

ColliderComponent::ColliderComponent () :
	_offset (0.0f),
	_collisionShape (nullptr),
	_collisionObject (new btCollisionObject ()),
	_isSelected (false),
	_isSelectedLastFrame (false)
{

}

ColliderComponent::~ColliderComponent ()
{
	delete _collisionObject;
	delete _collisionShape;
}

void ColliderComponent::Awake ()
{
	SetActive (_parent->IsActive ());
}

void ColliderComponent::Update ()
{
	if (_parent->GetTransform ()->IsDirty ()) {
		btTransform worldTransform;

		glm::vec3 offset = _parent->GetTransform ()->GetRotation () * _offset;

		glm::vec3 position = (_parent->GetTransform ()->GetPosition () + offset);
		glm::quat rotation = _parent->GetTransform ()->GetRotation ();

		worldTransform.setOrigin (btVector3 (position.x, position.y, position.z));
		worldTransform.setRotation (btQuaternion (rotation.x, rotation.y, rotation.z, rotation.w));

		_collisionObject->setWorldTransform (worldTransform);		
	}

	if (_isSelected == false) {
		_collisionObject->setCollisionFlags (_collisionObject->getCollisionFlags () | btCollisionObject::CF_DISABLE_VISUALIZE_OBJECT);

		_isSelectedLastFrame = false;
	}

	_isSelected = false;
}

void ColliderComponent::SetActive (bool isActive)
{
	int flags = _collisionObject->getActivationState ();
	flags = isActive ? flags &~ DISABLE_SIMULATION : flags | DISABLE_SIMULATION;

	_collisionObject->setActivationState (flags);
}

void ColliderComponent::OnGizmo ()
{
	_isSelected = true;

	if (_isSelectedLastFrame == false) {
		_collisionObject->setCollisionFlags (_collisionObject->getCollisionFlags () &~ btCollisionObject::CF_DISABLE_VISUALIZE_OBJECT);
		_isSelectedLastFrame = true;
	}
}

void ColliderComponent::OnAttachedToScene ()
{
	_collisionObject->setCollisionShape (_collisionShape);
	// _collisionObject->setCollisionFlags (_collisionObject->getCollisionFlags() | btCollisionObject::CF_STATIC_OBJECT);

	/*
	 * Attach collisionObject to physics system
	*/

	PhysicsManager::Instance ()->AttachCollisionObject (_collisionObject);
}

void ColliderComponent::OnDetachedFromScene ()
{
	/*
	 * Detach collision object from the physics system
	*/

	PhysicsManager::Instance ()->DetachCollisionObject (_collisionObject);
}

void ColliderComponent::SetModel (const Resource<Model>& model)
{

}

void ColliderComponent::SetOffset (const glm::vec3& offset)
{
	_offset = offset;
}

btCollisionShape* ColliderComponent::GetCollisionShape () const
{
	return _collisionShape;
}

const glm::vec3& ColliderComponent::GetOffset () const
{
	return _offset;
}
