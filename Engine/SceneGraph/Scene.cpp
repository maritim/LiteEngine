#include <vector>
#include <string>
#include <algorithm>

#include "Scene.h"

#include "SceneObject.h"

#include "Core/Console/Console.h"

Scene::Scene () :
	_sceneObjects (),
	_name (""),
	_skybox (nullptr),
	_boundingBox (new AABBVolume (new AABBVolume::AABBVolumeInformation ()))
{

}

void Scene::Init ()
{

}

void Scene::SetSkybox (Skybox *skybox)
{
	_skybox = skybox;
}

Skybox* Scene::GetSkybox () const
{
	return _skybox;
}

void Scene::AttachObject (SceneObject* object)
{
	_sceneObjects.push_back (object);

	object->OnAttachedToScene ();

	/*
	 * Update scene bounding box
	*/

	UpdateBoundingBox (object);
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

	// TODO: Recalculate Bounding Box when detach object
}

SceneObject* Scene::GetObject (const std::string& name) const
{
	for (SceneObject* sceneObject : *this) {
		if (sceneObject->GetName () == name) {
			return sceneObject;
		}
	}

	Console::LogError ("There is no object with name " + name + " in scene.");

	return nullptr;
}

SceneObject* Scene::GetObject (std::size_t index) const
{
	if (index >= _sceneObjects.size ()) {
		Console::LogWarning ("Scene object index exceed objects count for " + _name + ". " +
			std::to_string (_sceneObjects.size ()) + " < " + std::to_string (index));

		return nullptr;
	}

	return _sceneObjects [index];
}

std::size_t Scene::GetObjectsCount () const
{
	return _sceneObjects.size();
}

AABBVolume* Scene::GetBoundingBox () const
{
	return _boundingBox;
}

void Scene::Update()
{
	for (SceneObject* sceneObject : *this) {
		sceneObject->Update ();
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

	delete _skybox;
	delete _boundingBox;

	_sceneObjects.clear ();
	_sceneObjects.shrink_to_fit ();
}

void Scene::UpdateBoundingBox (SceneObject* sceneObject)
{
	/*
	 * Check only objects that have collider
	*/

	if (sceneObject->GetCollider () == nullptr) {
		return;
	}

	AABBVolume::AABBVolumeInformation* volume = _boundingBox->GetVolumeInformation ();

	GeometricPrimitive* sceneObjectVolumePrimitive = sceneObject->GetCollider ()->GetGeometricPrimitive ();
	AABBVolume* sceneObjectBoundingBox = dynamic_cast<AABBVolume*> (sceneObjectVolumePrimitive);
	AABBVolume::AABBVolumeInformation* sceneObjectVolume = sceneObjectBoundingBox->GetVolumeInformation ();

	volume->minVertex = glm::vec3 (std::min (volume->minVertex.x, sceneObjectVolume->minVertex.x),
		std::min (volume->minVertex.y, sceneObjectVolume->minVertex.y),
		std::min (volume->minVertex.z, sceneObjectVolume->minVertex.z));
	volume->maxVertex = glm::vec3 (std::max (volume->maxVertex.x, sceneObjectVolume->maxVertex.x),
		std::max (volume->maxVertex.y, sceneObjectVolume->maxVertex.y),
		std::max (volume->maxVertex.z, sceneObjectVolume->maxVertex.z));
}

SceneIterator Scene::begin () const
{
	return SceneIterator (this, 0);
}

SceneIterator Scene::end () const
{
	return SceneIterator (this, _sceneObjects.size ());
}
