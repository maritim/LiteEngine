#include "LightMapGameObject.h"

#include "SceneNodes/SceneLayer.h"

#include "Renderer/RenderSystem.h"

LightMapGameObject::LightMapGameObject () :
	GameObject ()
{
	_sceneLayers = (int) SceneLayer::STATIC | (int) SceneLayer::LIGHT_MAP;
}

LightMapGameObject::~LightMapGameObject ()
{

}

void LightMapGameObject::AttachMesh (const Resource<Model>& mesh)
{
	_mesh = mesh;

	Resource<ModelView> modelView = RenderSystem::LoadLightMapModel (mesh);

	_renderObject->SetTransform (_transform);
	_renderObject->SetModelView (modelView);
	_renderObject->SetRenderStage (RenderStage::RENDER_STAGE_DEFERRED);
	_renderObject->SetSceneLayers (_sceneLayers);
	_renderObject->SetPriority (1);
	_renderObject->SetCollider (_collider);

	if (_rigidbody->GetCollider () != nullptr) {
		_rigidbody->GetCollider ()->SetMesh (_mesh);
	}

	_collider->Rebuild (_mesh, _transform);
}
