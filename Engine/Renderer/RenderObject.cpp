#include "RenderObject.h"

#include "Renderer/Pipeline.h"

RenderObject::RenderObject () :
	_transform (nullptr),
	_modelView (nullptr),
	_renderStage (RenderStage::RENDER_STAGE_DEFERRED),
	_sceneLayers (0),
	_priority (0),
	_isActive (true),
	_modelSpaceBoundingBox (),
	_worldSpaceBoundingBox ()
{

}

void RenderObject::Update ()
{
	if (_transform == nullptr) {
		return;
	}

	if (_transform->IsDirty () == true) {
		UpdateBoundingBox ();
	}
}

void RenderObject::SetTransform (const Transform* transform)
{
	_transform = transform;

	UpdateBoundingBox ();
}

void RenderObject::SetModelView (const Resource<ModelView>& modelView)
{
	_modelView = modelView;
}

void RenderObject::SetBoundingBox (const AABBVolume& boundingBox)
{
	_modelSpaceBoundingBox = boundingBox;

	UpdateBoundingBox ();
}

void RenderObject::SetRenderStage (RenderStage renderStage)
{
	_renderStage = renderStage;
}

void RenderObject::SetSceneLayers (int sceneLayers)
{
	_sceneLayers = sceneLayers;
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

const Transform* RenderObject::GetTransform () const
{
	return _transform;
}

Resource<ModelView> RenderObject::GetModelView () const
{
	return _modelView;
}

const AABBVolume& RenderObject::GetBoundingBox () const
{
	return _worldSpaceBoundingBox;
}

RenderStage RenderObject::GetRenderStage () const
{
	return _renderStage;
}

int RenderObject::GetSceneLayers () const
{
	return _sceneLayers;
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

void RenderObject::DrawGeometry ()
{
	Pipeline::SetObjectTransform (_transform);

	_modelView->DrawGeometry ();
}

void RenderObject::UpdateBoundingBox ()
{
	if (_transform == nullptr) {
		return;
	}

	_worldSpaceBoundingBox.minVertex = glm::vec3 (std::numeric_limits<float>::infinity ());
	_worldSpaceBoundingBox.maxVertex = glm::vec3 (-std::numeric_limits<float>::infinity ());

	/*
	 * Calculate model matrix.
	*/

	const glm::mat4& modelMatrix = _transform->GetModelMatrix ();

	/*
	 * Calculate AABB from model bounding box
	*/

	float vertices [2][3] = {
		{
			_modelSpaceBoundingBox.minVertex.x,
			_modelSpaceBoundingBox.minVertex.y,
			_modelSpaceBoundingBox.minVertex.z
		},
		{
			_modelSpaceBoundingBox.maxVertex.x,
			_modelSpaceBoundingBox.maxVertex.y,
			_modelSpaceBoundingBox.maxVertex.z
		}
	};

	for (std::size_t i = 0; i < 2; i++) {
		for (std::size_t j = 0; j < 2; j++) {
			for (std::size_t k = 0; k < 2; k++) {

				glm::vec3 vertex = glm::vec3 (vertices [i][0], vertices [j][1], vertices [k][2]);

				vertex = glm::vec3 (modelMatrix * glm::vec4 (vertex, 1));

				_worldSpaceBoundingBox.minVertex.x = std::min (_worldSpaceBoundingBox.minVertex.x, vertex.x);
				_worldSpaceBoundingBox.minVertex.y = std::min (_worldSpaceBoundingBox.minVertex.y, vertex.y);
				_worldSpaceBoundingBox.minVertex.z = std::min (_worldSpaceBoundingBox.minVertex.z, vertex.z);
				_worldSpaceBoundingBox.maxVertex.x = std::max (_worldSpaceBoundingBox.maxVertex.x, vertex.x);
				_worldSpaceBoundingBox.maxVertex.y = std::max (_worldSpaceBoundingBox.maxVertex.y, vertex.y);
				_worldSpaceBoundingBox.maxVertex.z = std::max (_worldSpaceBoundingBox.maxVertex.z, vertex.z);
			}
		}
	}
}
