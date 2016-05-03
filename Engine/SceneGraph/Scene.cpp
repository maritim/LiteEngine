#include <vector>
#include <string>
#include <algorithm>

#include "Scene.h"

#include "SceneObject.h"

#include "Systems/Physics/PhysicsSystem.h"

#include "Core/Console/Console.h"

Scene::Scene () :
	_name (""),
	_sceneObjects ()
{

}

void Scene::Init ()
{
	Skybox::Set (_skybox);

	PhysicsSystem::Instance ().Init (this);
}

void Scene::SetSkybox (Skybox *sky)
{
	_skybox = sky;
}

Skybox* Scene::GetSkybox () const
{
	return _skybox;
}

void Scene::AttachObject (SceneObject* object)
{
	_sceneObjects.push_back (object);

	object->OnAttachedToScene ();
}

void Scene::DetachObject (SceneObject* object)
{
	auto it = std::find (_sceneObjects.begin (), _sceneObjects.end (), object);

	if (it == _sceneObjects.end ()) {
		Console::LogWarning ("Try to remove an invalid object. Something must be wrong here...");

		return ;
	}

	_sceneObjects.erase (it);

	(*it)->OnDetachedFromScene ();
}

SceneObject* Scene::GetObjectAt (std::size_t index) const
{
	if (index > _sceneObjects.size ()) {
		return nullptr;
	}

	return _sceneObjects [index];
}

SceneObject* Scene::GetObject (const std::string& name) const
{
	for (SceneObject* object : _sceneObjects) {
		if (object->GetName () == name) {
			return object;
		}
	}

	Console::LogError ("There is no object with name " + name + " in scene.");

	return nullptr;
}

std::size_t Scene::GetObjectsCount () const
{
	return _sceneObjects.size();
}

void Scene::Update()
{
	for(std::size_t i=0;i<_sceneObjects.size();i++) {
		_sceneObjects[i]->Update ();
	}
}

void Scene::SetName (const std::string& name)
{
	_name = name;
}

std::string Scene::GetName () const
{
	return _name;
}

Scene::~Scene ()
{
	for (std::size_t i=0;i<_sceneObjects.size ();i++) {
		_sceneObjects [i]->OnDetachedFromScene ();

		delete _sceneObjects [i];
	}

	_sceneObjects.clear ();
	_sceneObjects.shrink_to_fit ();
}