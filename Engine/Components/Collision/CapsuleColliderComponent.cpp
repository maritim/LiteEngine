#include "CapsuleColliderComponent.h"

#include <glm/geometric.hpp>
#include <BulletCollision/CollisionShapes/btCapsuleShape.h>

void CapsuleColliderComponent::Awake ()
{
	SetSize (_radius, _height);
}

void CapsuleColliderComponent::SetModel (const Resource<Model>& model)
{
	ColliderComponent::SetModel (model);

	/*
	 * Compute the bounding capsule extents of the mesh
	*/

	glm::vec2 size = GetSize (model);

	/*
	 * Bullet capsule shape is created based on radius and height
	*/

	SetSize (size.x, size.y);
}

void CapsuleColliderComponent::SetRadius (float radius)
{
	_radius = radius;

	SetSize (_radius, _height);
}

void CapsuleColliderComponent::SetHeight (float height)
{
	_height = height;

	SetSize (_radius, _height);
}

void CapsuleColliderComponent::SetSize (float radius, float height)
{
	_radius = radius;
	_height = height;

	OnDetachedFromScene ();

	/*
	 * Destroy current collision shape if exists
	*/

	delete _collisionShape;

	/*
	 * Bullet capsule shape is created based on radius and height
	*/

	_collisionShape = new btCapsuleShape (_radius, _height);

	OnAttachedToScene ();
}

float CapsuleColliderComponent::GetHeight () const
{
	return _height;
}

glm::vec2 CapsuleColliderComponent::GetSize (const Resource<Model>& model)
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

	return glm::vec2 (radius, height);
}
