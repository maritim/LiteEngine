#include "RenderScene.h"

#include <algorithm>

RenderScene::RenderScene () :
	_renderObjects (),
	_renderSkyboxObject (nullptr),
	_renderDirectionalLightObject (nullptr),
	_renderPointLightObjects (),
	_renderSpotLightObjects (),
	_renderAmbientLightObject (nullptr),
	_boundingBox ()
{

}

RenderScene::~RenderScene ()
{

}

void RenderScene::SetRenderSkyboxObject (RenderSkyboxObject* renderSkyboxObject)
{
	_renderSkyboxObject = renderSkyboxObject;
}

void RenderScene::AttachRenderObject (RenderObject* renderObject)
{
	_renderObjects.insert (renderObject);

	UpdateBoundingBox ();
}

void RenderScene::DetachRenderObject (RenderObject* renderObject)
{
	_renderObjects.erase (renderObject);

	UpdateBoundingBox ();
}

void RenderScene::SetRenderDirectionalLightObject (RenderDirectionalLightObject* renderDirectionalLightObject)
{
	_renderDirectionalLightObject = renderDirectionalLightObject;
}

void RenderScene::AttachRenderPointLightObject (RenderPointLightObject* renderPointLightObject)
{
	_renderPointLightObjects.insert (renderPointLightObject);
}

void RenderScene::DetachRenderPointLightObject (RenderPointLightObject* renderPointLightObject)
{
	_renderPointLightObjects.erase (renderPointLightObject);
}

void RenderScene::AttachRenderSpotLightObject (RenderSpotLightObject* renderSpotLightObject)
{
	_renderSpotLightObjects.insert (renderSpotLightObject);
}

void RenderScene::DetachRenderSpotLightObject (RenderSpotLightObject* renderSpotLightObject)
{
	_renderSpotLightObjects.erase (renderSpotLightObject);
}

void RenderScene::SetRenderAmbientLightObject (RenderAmbientLightObject* renderAmbientLightObject)
{
	_renderAmbientLightObject = renderAmbientLightObject;
}

RenderSkyboxObject* RenderScene::GetRenderSkyboxObject () const
{
	return _renderSkyboxObject;
}

RenderDirectionalLightObject* RenderScene::GetRenderDirectionalLightObject () const
{
	return _renderDirectionalLightObject;
}

RenderAmbientLightObject* RenderScene::GetRenderAmbientLightObject () const
{
	return _renderAmbientLightObject;
}

const AABBVolume& RenderScene::GetBoundingBox () const
{
	return _boundingBox;
}

void RenderScene::UpdateBoundingBox ()
{
	_boundingBox.minVertex = glm::vec3 (std::numeric_limits<float>::infinity ());
	_boundingBox.maxVertex = glm::vec3 (-std::numeric_limits<float>::infinity ());

	for (auto renderObject : _renderObjects) {

		auto& renderObjectBoundingBox = renderObject->GetBoundingBox ();

		_boundingBox.minVertex = glm::vec3 (
			std::min (_boundingBox.minVertex.x, renderObjectBoundingBox.minVertex.x),
			std::min (_boundingBox.minVertex.y, renderObjectBoundingBox.minVertex.y),
			std::min (_boundingBox.minVertex.z, renderObjectBoundingBox.minVertex.z)
		);
		_boundingBox.maxVertex = glm::vec3 (
			std::max (_boundingBox.maxVertex.x, renderObjectBoundingBox.maxVertex.x),
			std::max (_boundingBox.maxVertex.y, renderObjectBoundingBox.maxVertex.y),
			std::max (_boundingBox.maxVertex.z, renderObjectBoundingBox.maxVertex.z)
		);
	}
}
