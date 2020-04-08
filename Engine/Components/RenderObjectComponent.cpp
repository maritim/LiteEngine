#include "RenderObjectComponent.h"

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
	_renderObject->SetTransform (_parent->GetTransform ());
}

void RenderObjectComponent::SetActive (bool isActive)
{
	_renderObject->SetActive (isActive);
}

void RenderObjectComponent::OnAttachedToScene ()
{
	RenderManager::Instance ()->AttachRenderObject (_renderObject);
}

void RenderObjectComponent::OnDetachedFromScene ()
{
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
