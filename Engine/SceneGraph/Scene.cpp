#include <vector>
#include <string>
#include <algorithm>

#include "Scene.h"

#include "SceneObject.h"

#include "Core/Console/Console.h"

Scene::Scene () :
	_sceneRoot (new SceneRoot ()),
	_name (""),
	_skybox (nullptr),
	_boundingBox ()
{

}

SceneRoot* Scene::GetRoot () const
{
	return _sceneRoot;
}

void Scene::SetSkybox (Skybox *skybox)
{
	if (_skybox != nullptr) {
		_skybox->OnDetachedFromScene ();

		delete _skybox;
	}

	_skybox = skybox;

	_skybox->OnAttachedToScene ();
}

Skybox* Scene::GetSkybox () const
{
	return _skybox;
}

void Scene::AttachObject (SceneObject* object)
{
	if (object->GetTransform ()->GetParent () == nullptr) {
		object->GetTransform ()->SetParent (_sceneRoot->GetTransform ());
	}

	object->OnAttachedToScene ();

	/*
	 * Update scene bounding box
	*/

	UpdateBoundingBox (object);
}

void Scene::DetachObject (SceneObject* object)
{
	object->GetTransform ()->DetachParent ();

	object->OnDetachedFromScene ();

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

const AABBVolume& Scene::GetBoundingBox () const
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
	Clear (_sceneRoot);

	delete _skybox;
}

void Scene::UpdateBoundingBox (SceneObject* sceneObject)
{
	// AABBVolume::AABBVolumeInformation* volume = _boundingBox->GetVolumeInformation ();

	// GeometricPrimitive* sceneObjectVolumePrimitive = gameObject->GetCollider ()->GetGeometricPrimitive ();
	// AABBVolume* sceneObjectBoundingBox = dynamic_cast<AABBVolume*> (sceneObjectVolumePrimitive);
	// AABBVolume::AABBVolumeInformation* sceneObjectVolume = sceneObjectBoundingBox->GetVolumeInformation ();

	// volume->minVertex = glm::vec3 (std::min (volume->minVertex.x, sceneObjectVolume->minVertex.x),
	// 	std::min (volume->minVertex.y, sceneObjectVolume->minVertex.y),
	// 	std::min (volume->minVertex.z, sceneObjectVolume->minVertex.z));
	// volume->maxVertex = glm::vec3 (std::max (volume->maxVertex.x, sceneObjectVolume->maxVertex.x),
	// 	std::max (volume->maxVertex.y, sceneObjectVolume->maxVertex.y),
	// 	std::max (volume->maxVertex.z, sceneObjectVolume->maxVertex.z));
}

void Scene::Clear (SceneObject* object)
{
	std::vector<Transform*> children;

	for_each_type (Transform*, child, *object->GetTransform ()) {
		children.push_back (child);
	}

	for (auto child : children) {
		Clear (child->GetSceneObject ());
	}

	object->GetTransform ()->DetachParent ();
	object->OnDetachedFromScene ();

	delete object;
}

SceneIterator Scene::begin () const
{
	return SceneIterator (_sceneRoot->GetTransform ());
}

SceneIterator Scene::end () const
{
	return SceneIterator (nullptr);
}
