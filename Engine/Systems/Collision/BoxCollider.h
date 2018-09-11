#ifndef BOXCOLLIDER_H
#define BOXCOLLIDER_H

#include "BulletCollider.h"

#include <bullet/BulletCollision/CollisionShapes/btBoxShape.h>

class BoxCollider : public BulletCollider
{
protected:
	bool _isGenerated;

public:
	BoxCollider ();
	BoxCollider (const glm::vec3& extents);

	void Rebuild (Model* mesh, Transform* transform);
protected:
	glm::vec3 GetExtents (Model* mesh, Transform* transform);
};

#endif