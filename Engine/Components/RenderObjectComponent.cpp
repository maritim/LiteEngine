#include "RenderObjectComponent.h"

#include "Renderer/RenderAnimationObject.h"
#include "Renderer/RenderSystem.h"
#include "Renderer/RenderManager.h"

#include "SceneNodes/SceneLayer.h"

RenderObjectComponent::RenderObjectComponent () :
	_model (nullptr),
	_renderObject (nullptr)
{

}

RenderObjectComponent::~RenderObjectComponent ()
{
	delete _renderObject;
}

void RenderObjectComponent::Awake ()
{
	if (_model == nullptr) {
		return;
	}

	if (_layer & SceneLayer::ANIMATION) {
		_renderObject = new RenderAnimationObject ();

		_renderObject->SetSceneLayers (_layer);

		auto renderObject = (RenderAnimationObject*) _renderObject;
		renderObject->SetAnimationModel (_model);
	} else {
		_renderObject = new RenderObject ();

		_renderObject->SetSceneLayers (_layer | SceneLayer::STATIC);
	}

	/*
	 * Set render object model view
	*/

	Resource<ModelView> modelView = nullptr;

	if (_layer & SceneLayer::ANIMATION) {
		modelView = RenderSystem::LoadAnimationModel (_model);
	}

	if (_layer & SceneLayer::STATIC) {
		modelView = RenderSystem::LoadModel (_model);
	}

	if (_layer & SceneLayer::NORMAL_MAP) {
		modelView = RenderSystem::LoadNormalMapModel (_model);
	}

	if (_layer & SceneLayer::LIGHT_MAP) {
		modelView = RenderSystem::LoadLightMapModel (_model);
	}

	_renderObject->SetModelView (modelView);

	_renderObject->SetTransform (_parent->GetTransform ());
	_renderObject->SetRenderStage (RenderStage::RENDER_STAGE_DEFERRED);
	_renderObject->SetActive (_parent->IsActive ());

	RenderManager::Instance ()->AttachRenderObject (_renderObject);
}

void RenderObjectComponent::SetActive (bool isActive)
{
	_renderObject->SetActive (isActive);
}

void RenderObjectComponent::OnAttachedToScene ()
{
	if (_renderObject == nullptr) {
		return;
	}

	RenderManager::Instance ()->AttachRenderObject (_renderObject);
}

void RenderObjectComponent::OnDetachedFromScene ()
{
	if (_renderObject == nullptr) {
		return;
	}

	RenderManager::Instance ()->DetachRenderObject (_renderObject);
}

void RenderObjectComponent::SetModel (const Resource<Model>& model)
{
	_model = model;
}

void RenderObjectComponent::SetRenderObject (RenderObject* renderObject)
{
	_renderObject = renderObject;
}

const Resource<Model>& RenderObjectComponent::GetModel () const
{
	return _model;
}

RenderObject* RenderObjectComponent::GetRenderObject ()
{
	return _renderObject;
}
