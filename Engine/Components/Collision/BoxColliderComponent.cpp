#include "BoxColliderComponent.h"

#include <BulletCollision/CollisionShapes/btBoxShape.h>

#include "SceneGraph/SceneObject.h"

void BoxColliderComponent::Awake ()
{
	SetExtents (_extents);
}

void BoxColliderComponent::SetModel (const Resource<Model>& model)
{
	ColliderComponent::SetModel (model);

	/*
	 * Compute model extents
	*/

	auto extents = GetExtents (model);

	/*
	 * Compute the bounding cylinder extents according to scale
	*/

	extents = extents * _parent->GetTransform ()->GetScale ();

	/*
	 * Bullet box shape is created based on half of the extents
	*/

	SetExtents (extents);
}

void BoxColliderComponent::SetExtents (const glm::vec3& extents)
{
	_extents = extents;

	OnDetachedFromScene ();

	/*
	 * Destroy current collision shape if exists
	*/

	delete _collisionShape;

	/*
	 * Bullet box shape is created based on half of the extents
	*/

	btVector3 halfExtents = btVector3 (
		_extents.x / 2.0f,
		_extents.y / 2.0f,
		_extents.z / 2.0f
	);

	_collisionShape = new btBoxShape (halfExtents);

	OnAttachedToScene ();
}

const glm::vec3& BoxColliderComponent::GetExtents () const
{
	return _extents;
}

glm::vec3 BoxColliderComponent::GetExtents (const Resource<Model>& model)
{
	/*
	 * Get model bounding box
	*/

	auto& boundingBox = model->GetBoundingBox ();

	/*
	 * Compute model extents according to its world position
	*/

	glm::vec3 minVertex = glm::vec3 (boundingBox.xmin, boundingBox.ymin, boundingBox.zmin);
	glm::vec3 maxVertex = glm::vec3 (boundingBox.xmax, boundingBox.ymax, boundingBox.zmax);

	glm::vec3 extents = maxVertex - minVertex;

	return extents;
}
