#include "SphereCollider.h"

SphereCollider::SphereCollider () :
	_isGenerated (true)
{

}

SphereCollider::SphereCollider (float radius) :
	_isGenerated (false)
{
	/*
	 * Bullet sphere shape is created based on radius
	*/

	_collisionShape = new btSphereShape (radius);
}

void SphereCollider::Rebuild ()
{
	if (!_isGenerated || _mesh == nullptr) {
		return;
	}

	/*
	 * Destroy current collision shape if exists
	*/

	DestroyCollisionShape ();

	/*
	 * Compute the bounding sphere radius of the mesh
	*/

	float radius = GetRadius ();

	/*
	 * Bullet sphere shape is created based on radius
	*/

	_collisionShape = new btSphereShape (radius);
}

float SphereCollider::GetRadius ()
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
	 * Compute sphere height according to its actual size
	*/

	float radius = glm::length (extents / 2.0f);

	return radius;
}