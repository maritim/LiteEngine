#include "CylinderCollider.h"

CylinderCollider::CylinderCollider () :
	_isGenerated (true)
{

}

CylinderCollider::CylinderCollider (float radius, float height) :
	_isGenerated (false)
{
	/*
	 * Bullet cylinder shape is created based on half of the extents
	*/

	btVector3 halfExtents = btVector3 (
		radius,
		height / 2.0f,
		radius
	);

	_collisionShape = new btCylinderShape (halfExtents);
}

void CylinderCollider::Rebuild ()
{
	if (!_isGenerated || _mesh == nullptr) {
		return;
	}

	/*
	 * Destroy current collision shape if exists
	*/

	DestroyCollisionShape ();

	/*
	 * Compute the bounding cylinder extents of the mesh
	*/

	glm::vec3 extents = GetExtents ();

	/*
	 * Bullet cylinder shape is created based on half of the extents
	*/

	btVector3 halfExtents = btVector3 (
		extents.x / 2.0f,
		extents.y / 2.0f,
		extents.z / 2.0f
	);

	_collisionShape = new btCylinderShape (halfExtents);
}

glm::vec3 CylinderCollider::GetExtents ()
{
	/*
	 * Get mesh bounding box
	*/

	BoundingBox* boundingBox = _mesh->GetBoundingBox ();

	/*
	 * Compute mesh extents according to its world position
	*/

	glm::vec3 minVertex = glm::vec3 (boundingBox->xmin, boundingBox->ymin, boundingBox->zmin);
	glm::vec3 maxVertex = glm::vec3 (boundingBox->xmax, boundingBox->ymax, boundingBox->zmax);

	glm::vec3 extents = maxVertex - minVertex;

	return extents;
}