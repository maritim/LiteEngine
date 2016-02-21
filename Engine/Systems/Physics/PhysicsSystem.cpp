#include "PhysicsSystem.h"

#include "SceneGraph/SceneObject.h"
#include "Rigidbody.h"

PhysicsSystem::PhysicsSystem () :
	_currentScene (NULL)
{

}

PhysicsSystem& PhysicsSystem::Instance ()
{
	static PhysicsSystem physicsSystem;

	return physicsSystem;
}

void PhysicsSystem::Init (Scene* scene)
{
	_currentScene = scene;
}

void PhysicsSystem::UpdateScene ()
{
	for (std::size_t i=0;i<_currentScene->GetObjectsCount ();i++) {
		Rigidbody* rigidbody = _currentScene->GetObjectAt (i)->GetRigidbody ();

		rigidbody->Update ();
	}
}