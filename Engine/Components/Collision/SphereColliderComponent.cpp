#include "SphereColliderComponent.h"

#include <algorithm>
#include <glm/geometric.hpp>
#include <BulletCollision/CollisionShapes/btSphereShape.h>

#include "SceneGraph/SceneObject.h"

SphereColliderComponent::SphereColliderComponent () :
	_radius (1.0f)
{

}

void SphereColliderComponent::Awake ()
{
	SetRadius (_radius);
}

void SphereColliderComponent::SetModel (const Resource<Model>& model)
{
	ColliderComponent::SetModel (model);

	/*
	 * Compute the bounding sphere radius of the mesh
	*/

	float radius = GetRadius (model);

	/*
	 * Compute the bounding sphere radius according to scale
	*/

	auto& scale = _parent->GetTransform ()->GetScale ();
	float maxScale = std::max (scale.x, std::max (scale.y, scale.z));

	radius *= maxScale;

	/*
	 * Bullet sphere shape is created based on radius
	*/

	SetRadius (radius);
}

void SphereColliderComponent::SetRadius (float radius)
{
	_radius = radius;

	OnDetachedFromScene ();

	/*
	 * Destroy current collision shape if exists
	*/

	delete _collisionShape;

	/*
	 * Bullet sphere shape is created based on radius
	*/

	_collisionShape = new btSphereShape (_radius);

	OnAttachedToScene ();
}

float SphereColliderComponent::GetRadius () const
{
	return _radius;
}

float SphereColliderComponent::GetRadius (const Resource<Model>& model)
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
	 * Compute sphere height according to its actual size
	*/

	float radius = glm::length (extents / 2.0f);

	return radius;
}
