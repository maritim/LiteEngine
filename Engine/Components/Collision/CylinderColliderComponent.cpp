#include "CylinderColliderComponent.h"

#include <glm/geometric.hpp>
#include <bullet/BulletCollision/CollisionShapes/btCylinderShape.h>

#include "SceneGraph/SceneObject.h"

void CylinderColliderComponent::SetExtents (const glm::vec3& extents)
{
	_extents = extents;

	OnDetachedFromScene ();

	/*
	 * Destroy current collision shape if exists
	*/

	delete _collisionShape;

	/*
	 * Bullet cylinder shape is created based on half of the extents
	*/

	btVector3 halfExtents = btVector3 (
		extents.x / 2.0f,
		extents.y / 2.0f,
		extents.z / 2.0f
	);

	_collisionShape = new btCylinderShape (halfExtents);

	OnAttachedToScene ();
}
