#include "BoxCollider.h"

BoxCollider::BoxCollider (const Resource<Model>& model)
{
	Rebuild (model);
}

BoxCollider::BoxCollider (const glm::vec3& extents)
{
	/*
	 * Bullet box shape is created based on half of the extents
	*/

	btVector3 halfExtents = btVector3 (
		extents.x / 2.0f,
		extents.y / 2.0f,
		extents.z / 2.0f
	);

	_collisionShape = new btBoxShape (halfExtents);
}

void BoxCollider::Rebuild (const Resource<Model>& model)
{
	/*
	 * Destroy current collision shape if exists
	*/

	DestroyCollisionShape ();

	/*
	 * Compute mesh extents
	*/

	glm::vec3 extents = GetExtents (model);

	/*
	 * Bullet box shape is created based on half of the extents
	*/

	btVector3 halfExtents = btVector3 (
		extents.x / 2.0f,
		extents.y / 2.0f,
		extents.z / 2.0f
	);

	_collisionShape = new btBoxShape (halfExtents);
}

glm::vec3 BoxCollider::GetExtents (const Resource<Model>& model)
{
	/*
	 * Get mesh bounding box
	*/

	BoundingBox* boundingBox = model->GetBoundingBox ();

	/*
	 * Compute mesh extents according to its world position
	*/

	glm::vec3 minVertex = glm::vec3 (boundingBox->xmin, boundingBox->ymin, boundingBox->zmin);
	glm::vec3 maxVertex = glm::vec3 (boundingBox->xmax, boundingBox->ymax, boundingBox->zmax);

	glm::vec3 extents = maxVertex - minVertex;

	return extents;
}