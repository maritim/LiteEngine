#include "GameObject.h"

#include "SceneGraph/Transform.h"
#include "Model3DRenderer.h"
#include "Systems/Collision/AABBCollider.h"

#include "Systems/Input/Input.h"

GameObject::GameObject () :
	SceneObject (),
	_mesh (NULL)
{
	delete _renderer;
	_renderer = new Model3DRenderer (_transform);
	_renderer->SetPriority (1);

	_collider = new AABBCollider ();
}

#include "Debug/Logger/Logger.h"

void GameObject::AttachMesh (Model * mesh)
{
	DestroyCurrentMesh ();

	_mesh = mesh;

	Model3DRenderer* model3dRenderer = dynamic_cast<Model3DRenderer*>(_renderer);
	model3dRenderer->Attach (_mesh);

	if (_rigidbody->GetCollider () != nullptr) {
		_rigidbody->GetCollider ()->Rebuild (_mesh, _transform);
		_rigidbody->Rebuild ();
	}

	DEBUG_LOG (_name);
	_collider->Rebuild (_mesh, _transform);
}

Model* GameObject::GetMesh () const
{
	return _mesh;
}


void GameObject::Update() 
{
	if (_transform->IsDirty ()) {
		_collider->Rebuild (_mesh, _transform);

		if (_rigidbody != nullptr) {
			_rigidbody->Update ();
		}
	}

	_transform->SetIsDirty (false);
}

void GameObject::DestroyCurrentMesh ()
{
	if (!_mesh) {
		return;
	}

	delete _mesh;
}

GameObject::~GameObject ()
{
	DestroyCurrentMesh ();
}