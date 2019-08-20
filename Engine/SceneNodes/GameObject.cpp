#include "GameObject.h"

#include "SceneGraph/Transform.h"
#include "Systems/Collision/AABBCollider.h"

#include "Systems/Input/Input.h"

#include "Renderer/RenderSystem.h"

#include "SceneNodes/SceneLayer.h"

#include "Renderer/RenderManager.h"

#include "Mesh/AnimationModel.h"
#include "Mesh/LightMapModel.h"

GameObject::GameObject () :
	SceneObject (),
	_mesh (nullptr),
	_renderObject (new RenderObject ()),
	_rigidbody (new Rigidbody (_transform)),
	_collider (new AABBCollider ()),
	_audioSource (new AudioSource (_transform))
{

}

GameObject::~GameObject ()
{
	delete _renderObject;
	delete _rigidbody;
	delete _collider;
	delete _audioSource;
}

void GameObject::AttachMesh (const Resource<Model>& mesh)
{
	_mesh = mesh;

	Resource<ModelView> modelView = RenderSystem::LoadModel (mesh);

	_renderObject->SetTransform (_transform);
	_renderObject->SetModelView (modelView);
	_renderObject->SetRenderStage (RenderStage::RENDER_STAGE_DEFERRED);
	_renderObject->SetSceneLayers (SceneLayer::STATIC);
	_renderObject->SetPriority (1);
	_renderObject->SetCollider (_collider);

	if (_rigidbody->GetCollider () != nullptr) {
		_rigidbody->GetCollider ()->SetMesh (_mesh);
	}

	_collider->Rebuild (_mesh, _transform);
}

void GameObject::SetActive (bool isActive)
{
	SceneObject::SetActive (isActive);

	_rigidbody->Enable (_isActive);

	_renderObject->SetActive (_isActive);
}

Resource<Model> GameObject::GetMesh () const
{
	return _mesh;
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
		_collider->Rebuild (_mesh, _transform);

		_rigidbody->Update ();
		_audioSource->Update ();
	}

	_transform->SetIsDirty (false);
}

void GameObject::OnAttachedToScene ()
{
	_rigidbody->OnAttachedToScene ();

	RenderManager::Instance ()->AttachRenderObject (_renderObject);
}

void GameObject::OnDetachedFromScene ()
{
	_rigidbody->OnDetachedFromScene ();

	RenderManager::Instance ()->DetachRenderObject (_renderObject);
}
