#include <vector>
#include <string>
#include <algorithm>

#include "Scene.h"

#include "SceneObject.h"

#include "Systems/Physics/PhysicsSystem.h"

#include "Core/Console/Console.h"

Scene::Scene () :
	_sceneObjects (),
	_name (""),
	_boundingBox (new AABBVolume (new AABBVolume::AABBVolumeInformation ()))
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

AABBVolume* Scene::GetBoundingBox () const
{
	return _boundingBox;
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

#include "Debug/Logger/Logger.h"
#include "Core/Math/glm/gtx/string_cast.hpp"

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

	DEBUG_LOG ("Min vertex: " + glm::to_string (volume->minVertex));
	DEBUG_LOG ("Max vertex: " + glm::to_string (volume->maxVertex));
}