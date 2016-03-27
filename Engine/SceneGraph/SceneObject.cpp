#include "SceneObject.h"

#include "Transform.h"
#include "Renderer/Renderer.h"

SceneObject::SceneObject () :
	_transform (new Transform ()),
	_renderer (new Renderer (_transform)),
	_rigidbody (new Rigidbody (_transform))
{

}

SceneObject::~SceneObject ()
{
	delete _transform;
	delete _renderer;
	delete _rigidbody;
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

std::string SceneObject::GetName () const
{
	return _name;
}

std::size_t SceneObject::GetInstanceID () const
{
	return _instanceID;
}

void SceneObject::SetName (const std::string& name)
{
	_name = name;
}

void SceneObject::SetInstanceID (std::size_t instanceID)
{
	_instanceID = instanceID;
}

void SceneObject::OnAttachedToScene ()
{

}

void SceneObject::OnDetachedFromScene ()
{

}