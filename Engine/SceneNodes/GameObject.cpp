#include "GameObject.h"

#include "SceneGraph/Transform.h"
#include "Systems/Collision/AABBCollider.h"

GameObject::GameObject () :
	SceneObject (),
	_rigidbody (new Rigidbody (_transform)),
	_collider (new AABBCollider ()),
	_audioSource (new AudioSource (_transform))
{
	// if (_rigidbody->GetCollider () != nullptr) {
	// 	_rigidbody->GetCollider ()->SetMesh (_mesh);
	// }

	// _collider->Rebuild (_mesh, _transform);
}

GameObject::~GameObject ()
{
	delete _rigidbody;
	delete _collider;
	delete _audioSource;
}

void GameObject::SetActive (bool isActive)
{
	SceneObject::SetActive (isActive);

	_rigidbody->Enable (_isActive);
}

Rigidbody* GameObject::GetRigidbody () const
{
	return _rigidbody;
}

Collider* GameObject::GetCollider () const
{
	return _collider;
}

AudioSource* GameObject::GetAudioSource () const
{
	return _audioSource;
}

void GameObject::Update() 
{
	if (_transform->IsDirty ()) {
		// _collider->Rebuild (_mesh, _transform);

		_rigidbody->Update ();
		_audioSource->Update ();
	}

	_transform->SetIsDirty (false);
}

void GameObject::OnAttachedToScene ()
{
	SceneObject::OnAttachedToScene ();

	_rigidbody->OnAttachedToScene ();
}

void GameObject::OnDetachedFromScene ()
{
	SceneObject::OnDetachedFromScene ();

	_rigidbody->OnDetachedFromScene ();
}
