#include "SceneObject.h"

#include "Transform.h"
#include "Renderer/Renderer.h"
#include "SceneNodes/SceneLayer.h"

SceneObject::SceneObject () :
	_transform (new Transform ()),
	_renderer (new Renderer (_transform)),
	_rigidbody (new Rigidbody (_transform)),
	_collider (nullptr),
	_sceneLayers ((int) SceneLayer::STATIC),
	_isActive (true)
{

}

SceneObject::~SceneObject ()
{
	delete _transform;
	delete _renderer;
	delete _rigidbody;

	if (_collider != nullptr) {
		delete _collider;
	}
}

// Don't ever modify this object's pointer
Transform* SceneObject::GetTransform () const
{
	return _transform;
}

Renderer* SceneObject::GetRenderer () const
{
	return _renderer;
}

Rigidbody* SceneObject::GetRigidbody () const
{
	return _rigidbody;
}

Collider* SceneObject::GetCollider () const
{
	return _collider;
}

int SceneObject::GetLayers() const
{
	return _sceneLayers;
}

std::string SceneObject::GetName () const
{
	return _name;
}

std::size_t SceneObject::GetInstanceID () const
{
	return _instanceID;
}

bool SceneObject::IsActive () const
{
	return _isActive;
}

void SceneObject::SetName (const std::string& name)
{
	_name = name;
}

void SceneObject::SetInstanceID (std::size_t instanceID)
{
	_instanceID = instanceID;
}

void SceneObject::SetActive (bool isActive)
{
	_isActive = isActive;
}

void SceneObject::OnAttachedToScene ()
{

}

void SceneObject::OnDetachedFromScene ()
{

}