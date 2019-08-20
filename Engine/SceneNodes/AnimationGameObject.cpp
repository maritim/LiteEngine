#include "AnimationGameObject.h"

#include "SceneNodes/SceneLayer.h"

#include "Renderer/RenderSystem.h"

#include "Renderer/RenderAnimationObject.h"

AnimationGameObject::AnimationGameObject () :
	GameObject ()
{
	_sceneLayers = (int) SceneLayer::ANIMATION;
}

AnimationGameObject::~AnimationGameObject ()
{
	
}

void AnimationGameObject::AttachMesh (const Resource<Model>& mesh)
{
	_mesh = mesh;

	Resource<ModelView> modelView = RenderSystem::LoadAnimationModel (mesh);

	//TODO: Fix this
	delete _renderObject;
	_renderObject = new RenderAnimationObject ();

	_renderObject->SetTransform (_transform);
	_renderObject->SetModelView (modelView);
	_renderObject->SetRenderStage (RenderStage::RENDER_STAGE_DEFERRED);
	_renderObject->SetSceneLayers (SceneLayer::ANIMATION);
	_renderObject->SetPriority (1);
	_renderObject->SetCollider (_collider);

	((RenderAnimationObject*) _renderObject)->SetAnimationModel (mesh);

	if (_rigidbody->GetCollider () != nullptr) {
		_rigidbody->GetCollider ()->SetMesh (_mesh);
	}

	_collider->Rebuild (_mesh, _transform);
}

void AnimationGameObject::SetAnimationClip (const std::string& animName)
{
	((RenderAnimationObject*) _renderObject)->SetAnimationClip (animName);
}

void AnimationGameObject::Blend (const std::string& nextAnimName, float duration)
{
	((RenderAnimationObject*) _renderObject)->Blend (nextAnimName, duration);
}
