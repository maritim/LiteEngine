#include "SceneObject.h"

#include "Transform.h"
#include "Renderer/Renderer.h"
#include "SceneNodes/SceneLayer.h"

SceneObject::SceneObject () :
	_transform (new Transform ()),
	_sceneLayers ((int) SceneLayer::STATIC),
	_isActive (true)
{

}

SceneObject::~SceneObject ()
{
	delete _transform;
}

// Don't ever modify this object's pointer
Transform* SceneObject::GetTransform () const
{
	return _transform;
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