#include "Physics.h"

#include "PhysicsManager.h"

void Physics::Init ()
{
	PhysicsManager::Instance ()->Init ();
}

void Physics::Quit ()
{
	PhysicsManager::Instance ()->Clear ();
}