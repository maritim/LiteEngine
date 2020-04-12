#ifndef CYLINDERCOLLIDER_H
#define CYLINDERCOLLIDER_H

#include "BulletCollider.h"

#include <bullet/BulletCollision/CollisionShapes/btCylinderShape.h>

class ENGINE_API CylinderCollider : public BulletCollider
{
public:
	CylinderCollider (const Resource<Model>& model);
	CylinderCollider (float radius, float height);
protected:
	void Rebuild (const Resource<Model>& model);

	glm::vec3 GetExtents (const Resource<Model>& model);
};

#endif