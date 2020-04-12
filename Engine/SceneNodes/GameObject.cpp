#include "GameObject.h"

#include "SceneGraph/Transform.h"
#include "Systems/Collision/AABBCollider.h"

GameObject::GameObject () :
	SceneObject (),
	_collider (new AABBCollider ())
{
	// _collider->Rebuild (_mesh, _transform);
}

GameObject::~GameObject ()
{
	delete _collider;
}

void GameObject::SetActive (bool isActive)
{
	SceneObject::SetActive (isActive);
}

Collider* GameObject::GetCollider () const
{
	return _collider;
}

void GameObject::Update() 
{

}

void GameObject::OnAttachedToScene ()
{
	SceneObject::OnAttachedToScene ();
}

void GameObject::OnDetachedFromScene ()
{
	SceneObject::OnDetachedFromScene ();
}
