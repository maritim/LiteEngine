#include "CapsuleCollider.h"

CapsuleCollider::CapsuleCollider () :
	_isGenerated (true)
{

}

CapsuleCollider::CapsuleCollider (float radius, float height) :
	_isGenerated (false)
{
	/*
	 * Bullet capsule shape is created based on radius and height
	*/

	_collisionShape = new btCapsuleShape (radius, height);
}

void CapsuleCollider::Rebuild ()
{
	if (!_isGenerated || _mesh == nullptr) {
		return;
	}

	/*
	 * Destroy current collision shape if exists
	*/

	DestroyCollisionShape ();

	/*
	 * Compute the bounding capsule extents of the mesh
	*/

	std::pair<float, float> size = GetSize ();

	/*
	 * Bullet capsule shape is created based on radius and height
	*/

	_collisionShape = new btCapsuleShape (size.first, size.second);
}

std::pair<float, float> CapsuleCollider::GetSize ()
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

	/*
	 * Compute capsule height according to its actual size
	 *
	 * actualHeight = height + 2 * radius
	*/

	float radius = glm::length (glm::vec2 (extents.x, extents.z) / 2.0f);
	float height = extents.y - 2 * radius;

	return std::pair<float, float> (radius, height);
}