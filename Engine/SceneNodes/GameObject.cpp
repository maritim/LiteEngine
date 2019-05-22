#include "GameObject.h"

#include "SceneGraph/Transform.h"
#include "Model3DRenderer.h"
#include "Systems/Collision/AABBCollider.h"

#include "Systems/Input/Input.h"

GameObject::GameObject () :
	SceneObject (),
	_mesh (nullptr)
{
	delete _renderer;
	_renderer = new Model3DRenderer (_transform);
	_renderer->SetPriority (1);

	_collider = new AABBCollider ();
}

void GameObject::AttachMesh (const Resource<Model>& mesh)
{
	_mesh = mesh;

	Model3DRenderer* model3dRenderer = dynamic_cast<Model3DRenderer*>(_renderer);
	model3dRenderer->Attach (_mesh);

	if (_rigidbody->GetCollider () != nullptr) {
		_rigidbody->GetCollider ()->SetMesh (_mesh);
	}

	_collider->Rebuild (_mesh, _transform);
}

Resource<Model> GameObject::GetMesh () const
{
	return _mesh;
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

GameObject::~GameObject ()
{

}