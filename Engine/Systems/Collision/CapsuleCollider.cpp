#include "CapsuleCollider.h"

CapsuleCollider::CapsuleCollider (const Resource<Model>& model)
{
	Rebuild (model);
}

CapsuleCollider::CapsuleCollider (float radius, float height)
{
	/*
	 * Bullet capsule shape is created based on radius and height
	*/

	_collisionShape = new btCapsuleShape (radius, height);
}

void CapsuleCollider::Rebuild (const Resource<Model>& model)
{
	/*
	 * Destroy current collision shape if exists
	*/

	DestroyCollisionShape ();

	/*
	 * Compute the bounding capsule extents of the mesh
	*/

	std::pair<float, float> size = GetSize (model);

	/*
	 * Bullet capsule shape is created based on radius and height
	*/

	_collisionShape = new btCapsuleShape (size.first, size.second);
}

std::pair<float, float> CapsuleCollider::GetSize (const Resource<Model>& model)
{
	/*
	 * Get mesh bounding box
	*/

	auto& boundingBox = model->GetBoundingBox ();

	/*
	 * Compute mesh extents according to its world position
	*/

	glm::vec3 minVertex = glm::vec3 (boundingBox.xmin, boundingBox.ymin, boundingBox.zmin);
	glm::vec3 maxVertex = glm::vec3 (boundingBox.xmax, boundingBox.ymax, boundingBox.zmax);

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