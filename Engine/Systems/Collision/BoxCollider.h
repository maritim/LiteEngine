#ifndef BOXCOLLIDER_H
#define BOXCOLLIDER_H

#include "BulletCollider.h"

#include <bullet/BulletCollision/CollisionShapes/btBoxShape.h>

class ENGINE_API BoxCollider : public BulletCollider
{
public:
	BoxCollider (const Resource<Model>& model);
	BoxCollider (const glm::vec3& extents);
protected:
	void Rebuild (const Resource<Model>& model);

	glm::vec3 GetExtents (const Resource<Model>& model);
};

#endif