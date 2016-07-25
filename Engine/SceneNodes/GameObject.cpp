#include "GameObject.h"

#include "SceneGraph/Transform.h"
#include "Systems/Time/Time.h"
#include "Model3DRenderer.h"

#include "Systems/Input/Input.h"

GameObject::GameObject () :
	SceneObject (),
	_mesh (NULL)
{
	delete _renderer;
	_renderer = new Model3DRenderer (_transform);
	_renderer->SetPriority (1);
}

void GameObject::AttachMesh (Model * mesh)
{
	DestroyCurrentMesh ();

	_mesh = mesh;

	Model3DRenderer* model3dRenderer = dynamic_cast<Model3DRenderer*>(_renderer);
	model3dRenderer->Attach (_mesh);
}

Model* GameObject::GetMesh () const
{
	return _mesh;
}


void GameObject::Update() 
{

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