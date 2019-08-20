#include "RenderScene.h"

RenderScene::RenderScene () :
	_renderObjects (),
	_renderSkyboxObject (nullptr),
	_renderDirectionalLightObject (nullptr),
	_renderPointLightObjects (),
	_renderAmbientLightObject (nullptr),
	_boundingBox (new AABBVolume (new AABBVolume::AABBVolumeInformation ()))
{

}

RenderScene::~RenderScene ()
{
	delete _boundingBox;
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

AABBVolume* RenderScene::GetBoundingBox () const
{
	return _boundingBox;
}

std::set<RenderObject*>::const_iterator RenderScene::begin () const
{
	return _renderObjects.begin ();
}

std::set<RenderObject*>::const_iterator RenderScene::end () const
{
	return _renderObjects.end ();
}

void RenderScene::UpdateBoundingBox ()
{
	AABBVolume::AABBVolumeInformation* volume = _boundingBox->GetVolumeInformation ();

	volume->minVertex = glm::vec3 (std::numeric_limits<float>::infinity ());
	volume->maxVertex = glm::vec3 (-std::numeric_limits<float>::infinity ());

	for (auto renderObject : _renderObjects) {

		/*
		 * Check only objects that have collider
		*/

		if (renderObject->GetCollider () == nullptr) {
			return;
		}

		GeometricPrimitive* renderObjectVolumePrimitive = renderObject->GetCollider ()->GetGeometricPrimitive ();
		AABBVolume* renderObjectBoundingBox = dynamic_cast<AABBVolume*> (renderObjectVolumePrimitive);
		AABBVolume::AABBVolumeInformation* renderObjectVolume = renderObjectBoundingBox->GetVolumeInformation ();

		volume->minVertex = glm::vec3 (std::min (volume->minVertex.x, renderObjectVolume->minVertex.x),
			std::min (volume->minVertex.y, renderObjectVolume->minVertex.y),
			std::min (volume->minVertex.z, renderObjectVolume->minVertex.z));
		volume->maxVertex = glm::vec3 (std::max (volume->maxVertex.x, renderObjectVolume->maxVertex.x),
			std::max (volume->maxVertex.y, renderObjectVolume->maxVertex.y),
			std::max (volume->maxVertex.z, renderObjectVolume->maxVertex.z));
	}
}
