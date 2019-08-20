#include "RenderObject.h"

#include "Renderer/Pipeline.h"

RenderObject::RenderObject () :
	_transform (nullptr),
	_modelView (nullptr),
	_renderStage (RenderStage::RENDER_STAGE_DEFERRED),
	_sceneLayers (0),
	_priority (0),
	_collider (nullptr),
	_isActive (true)
{

}

void RenderObject::SetTransform (Transform* transform)
{
	_transform = transform;
}

void RenderObject::SetModelView (const Resource<ModelView>& modelView)
{
	_modelView = modelView;
}

void RenderObject::SetRenderStage (RenderStage renderStage)
{
	_renderStage = renderStage;
}

void RenderObject::SetSceneLayers (int sceneLayers)
{
	_sceneLayers = sceneLayers;
}

void RenderObject::SetCollider (Collider* collider)
{
	_collider = collider;
}

void RenderObject::SetPriority (int priority)
{
	_priority = priority;
}

void RenderObject::SetActive (bool isActive)
{
	_isActive = isActive;
}

void RenderObject::SetAttributes (const std::vector<PipelineAttribute>& attributes)
{
	_attributes = attributes;
}

Transform* RenderObject::GetTransform () const
{
	return _transform;
}

Resource<ModelView> RenderObject::GetModelView () const
{
	return _modelView;
}

RenderStage RenderObject::GetRenderStage () const
{
	return _renderStage;
}

int RenderObject::GetSceneLayers () const
{
	return _sceneLayers;
}

Collider* RenderObject::GetCollider () const
{
	return _collider;
}

int RenderObject::GetPriority () const
{
	return _priority;
}

bool RenderObject::IsActive () const
{
	return _isActive;
}

void RenderObject::Draw ()
{
	Pipeline::SetObjectTransform (_transform);

	_modelView->Draw ();
}