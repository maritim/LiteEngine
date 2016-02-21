#include <vector>
#include <string>

#include "Scene.h"

#include "SceneObject.h"

#include "Systems/Physics/PhysicsSystem.h"

#include "Core/Console/Console.h"

Scene::Scene () :
	_sceneObjects ()
{

}

void Scene::Init ()
{
	Skybox::Set (_skybox);

	PhysicsSystem::Instance ().Init (this);
}

Scene* Scene::Current ()
{
	static Scene* scene = new Scene ();

	return scene;
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
}

SceneObject* Scene::GetObjectAt (std::size_t index) const
{
	if (index > _sceneObjects.size ()) {
		return NULL;
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

	return NULL;
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

Scene::~Scene ()
{
	for (std::size_t i=0;i<_sceneObjects.size ();i++) {
		delete _sceneObjects [i];
	}

	_sceneObjects.clear ();
	std::vector<SceneObject*> ().swap (_sceneObjects);
}