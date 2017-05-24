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
	for (SceneObject* sceneObject : *_currentScene) {
		Rigidbody* rigidbody = sceneObject->GetRigidbody ();

		rigidbody->Update ();
	}
}